{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE TypeSynonymInstances #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE LambdaCase #-}

module Evaluation (evaluate) where

import Control.Monad.Identity
import Control.Monad.Reader (ReaderT, runReaderT, local, ask, asks)
import Control.Monad.Except (ExceptT, runExceptT, throwError)
import qualified Data.Map as Map
import Data.Map (Map)
import Data.Char (toLower)
import Built.AbsOSkell
import Err

newtype CustomIdentity a = CustomIdentity { runCustomIdentity :: a }

instance MonadFix CustomIdentity where
    mfix f = CustomIdentity (fix (runCustomIdentity . f))

instance Monad CustomIdentity where
    return = CustomIdentity
    m >>= k = k (runCustomIdentity m)

instance Applicative CustomIdentity where
    pure = return
    mf <*> ma = do
        f <- mf
        a <- ma
        return (f a)

instance Functor CustomIdentity where
    fmap f ma = pure f <*> ma

instance MonadFail CustomIdentity where
    fail = CustomIdentity . error

type TypeVar = String

-- Definition of different types in the language
data Type
    = TInt
    | TBool
    | TVar TypeVar
    | TFun Type Type
    deriving (Eq, Show)

type Environment = Map Ident Value

type EvalM a = ExceptT Err (ReaderT Environment CustomIdentity) a

runEvalM :: EvalM a -> Environment -> Either Err a
runEvalM im env = runCustomIdentity $ runReaderT (runExceptT im) env

data Value 
    = IntValue  Integer
    | BoolValue Bool
    | ListValue [Value] 
    | LambdaValue (Value -> EvalM Value) Environment
    | TypeValue Type

instance Show Value where
    show (IntValue int)   = show int
    show (BoolValue bool) = map toLower $ show bool
    show (ListValue xs)   = show xs
    show LambdaValue{}    = "<<lambda>>"
    show (TypeValue t)    = show t

stdlib :: Environment
stdlib = Map.fromList [
    (Ident "null", LambdaValue (\e -> case e of
        ListValue xs -> return $ BoolValue $ null xs
        _            -> throwError $ Err (Nothing, "Attempting to apply list function to a non-list value")
    ) Map.empty),
    (Ident "head", LambdaValue (\e -> case e of
        ListValue xs -> if null xs
                        then throwError $ Err (Nothing, "head: empty list")
                        else return $ head xs
        _            -> throwError $ Err (Nothing, "Attempting to apply list function to a non-list value")
    ) Map.empty),
    (Ident "tail", LambdaValue (\e -> case e of
        ListValue xs -> if null xs
                        then throwError $ Err (Nothing, "tail: empty list")
                        else return $ ListValue $ tail xs
        _            -> throwError $ Err (Nothing, "Attempting to apply list function to a non-list value")
    ) Map.empty)
  ]

evaluate :: Exp -> Either Err Value
evaluate e = runEvalM (eval e) stdlib

-- Evaluates boolean binary operations
evalBoolsToBool :: (Bool -> Bool -> Bool) -> Exp -> Exp -> EvalM Value
evalBoolsToBool op e1 e2 = do
    v1 <- eval e1
    v2 <- eval e2
    case (v1, v2) of
        (BoolValue b1, BoolValue b2) -> return $ BoolValue $ b1 `op` b2
        _                            -> throwError $ Err (Nothing, "Expected boolean values")

-- Evaluates integer comparison operations
evalIntsToBool :: (Integer -> Integer -> Bool) -> Exp -> Exp -> EvalM Value
evalIntsToBool op e1 e2 = do
    v1 <- eval e1
    v2 <- eval e2
    case (v1, v2) of
        (IntValue i1, IntValue i2) -> return $ BoolValue $ i1 `op` i2
        _                          -> throwError $ Err (Nothing, "Expected integer values")

-- Evaluates integer arithmetic operations with error checking
evalIntsToInt :: (Integer -> Integer -> Integer) -> Exp -> Exp -> (Integer -> Integer -> Maybe Err) -> EvalM Value
evalIntsToInt op e1 e2 check = do
    v1 <- eval e1
    v2 <- eval e2
    case (v1, v2) of
        (IntValue i1, IntValue i2) ->
            case check i1 i2 of 
                Just e  -> throwError e
                Nothing -> return $ IntValue $ i1 `op` i2
        _ -> throwError $ Err (Nothing, "Expected integer values")

-- Evaluates lambda expressions
evalLam :: Position -> Ident -> [Ident] -> Exp -> Value -> EvalM Value
evalLam position x xs e v = local (Map.insert x v) (eval $ ExpLambda position xs e)

-- Applies a parameter to a lambda
apParLambda :: Value -> Par -> EvalM Value
apParLambda (LambdaValue f env) x = 
    evalPar x >>= \v -> local (const env) (f v)
apParLambda _ _ = throwError $ Err (Nothing, "Attempting to apply a non-lambda value as a lambda")

-- Evaluates a parameter
evalPar :: Par -> EvalM Value
evalPar par = case par of
    ParInteger _ i                   -> return $ IntValue i
    ParTrue _                        -> return $ BoolValue True
    ParFalse _                       -> return $ BoolValue False
    ParConsComma position xs         -> eval $ ExpConsComma position xs
    ParConsRangeClosed position l r  -> eval $ ExpConsRangeClosed position l r
    ParConsRangeOpen position left   -> eval $ ExpConsRangeOpen position left
    ParFunction position f           -> eval $ ExpFunctionApplication position f []
    ParLambda position f             -> eval $ ExpLambdaApplication position f []

-- Evaluates if expressions
evalExpIf :: Exp -> Exp -> Exp -> EvalM Value
evalExpIf condExp whenTrueExp whenFalseExp = do
    BoolValue cond <- eval condExp
    eval (if cond then whenTrueExp else whenFalseExp)

-- Evaluates let expressions
evalExpLet :: Position -> Ident -> [Ident] -> Exp -> Exp -> EvalM Value
evalExpLet position x xs bodyExp expr = do
    mfix (evalLam position x xs bodyExp) >>= evalLam position x [] expr

-- Evaluates lambda expressions
evalExpLambda :: Position -> [Ident] -> Exp -> EvalM Value
evalExpLambda _ [] e = eval e
evalExpLambda position (x:xs) e = do
    env <- ask
    return $ LambdaValue (evalLam position x xs e) env

-- Evaluates not expressions
evalExpNot :: Exp -> EvalM Value
evalExpNot e = do
    BoolValue boolVal <- eval e
    return $ BoolValue $ not boolVal

-- Evaluates list concatenation
evalExpConcat :: Exp -> Exp -> EvalM Value
evalExpConcat exp1 exp2 = do
    ListValue list1 <- eval exp1
    ListValue list2 <- eval exp2
    return $ ListValue $ list1 ++ list2

-- Evaluates division with error handling for division by zero
evalExpDivide :: Position -> Exp -> Exp -> EvalM Value
evalExpDivide position dividendExp divisorExp = do
    IntValue dividend <- eval dividendExp
    IntValue divisor <- eval divisorExp
    if divisor == 0
        then throwError $ Err (position, "Division by zero")
        else return $ IntValue $ dividend `div` divisor

-- Evaluates modulo with error handling for division by zero
evalExpModulo :: Position -> Exp -> Exp -> EvalM Value
evalExpModulo position dividendExp moduloExp = do
    IntValue dividend <- eval dividendExp
    IntValue modulo <- eval moduloExp
    if modulo == 0
        then throwError $ Err (position, "Modulo by zero")
        else return $ IntValue $ dividend `mod` modulo

-- Evaluates power operation with error handling for negative exponents
evalExpPower :: Position -> Exp -> Exp -> EvalM Value
evalExpPower position baseExp exponentExp = do
    IntValue base <- eval baseExp
    IntValue exponentVal <- eval exponentExp
    if exponentVal < 0
        then throwError $ Err (position, "Negative exponent")
        else return $ IntValue $ base ^ exponentVal

-- Evaluates function applications
evalExpFunctionApplication :: Ident -> [Par] -> EvalM Value
evalExpFunctionApplication fun xs = 
    asks (Map.! fun) >>= \fun' -> foldM apParLambda fun' xs

-- Evaluates lambda applications
evalExpLambdaApplication :: Exp -> [Par] -> EvalM Value
evalExpLambdaApplication lam xs = 
    eval lam >>= \lam' -> foldM apParLambda lam' xs

-- Evaluates list construction with commas
evalExpConsComma :: [Exp] -> EvalM Value
evalExpConsComma xs = mapM eval xs >>= \vs -> return $ ListValue vs

-- Evaluates list construction with dots
evalExpConsDots :: Par -> Par -> EvalM Value
evalExpConsDots headPar restPar = do
    x <- evalPar headPar
    ListValue xs <- evalPar restPar
    return $ ListValue $ x : xs

-- Evaluates closed range list construction
evalExpConsRangeClosed :: Par -> Par -> EvalM Value
evalExpConsRangeClosed startPar endPar = do
    IntValue startValue <- evalPar startPar
    IntValue endValue <- evalPar endPar
    return $ ListValue $ map IntValue [startValue..endValue]

-- Evaluates open range list construction
evalExpConsRangeOpen :: Par -> EvalM Value
evalExpConsRangeOpen startPar = do
    IntValue startValue <- evalPar startPar
    return $ ListValue $ map IntValue [startValue..]

-- Main evaluation function
eval :: Exp -> EvalM Value
eval expr = 
    let noCheck = \_ _ -> Nothing
    in case expr of 
        ExpIf  _ cond whenTrue whenFalse   -> evalExpIf cond whenTrue whenFalse
        ExpLet      position x xs body e   -> evalExpLet position x xs body e
        ExpLambda   position xs e          -> evalExpLambda position xs e

        ExpAnd            _    e1 e2       -> evalBoolsToBool (&&)  e1 e2
        ExpOr             _    e1 e2       -> evalBoolsToBool (||)  e1 e2
        ExpNot            _    e           -> evalExpNot e
        ExpEqual          _    e1 e2       -> evalIntsToBool  (==)  e1 e2
        ExpNotEqual       _    e1 e2       -> evalIntsToBool  (/=)  e1 e2
        ExpLessThan       _    e1 e2       -> evalIntsToBool  (<)   e1 e2
        ExpGreaterThan    _    e1 e2       -> evalIntsToBool  (>)   e1 e2
        ExpLessEqual      _    e1 e2       -> evalIntsToBool  (<=)  e1 e2
        ExpGreaterEqual   _    e1 e2       -> evalIntsToBool  (>=)  e1 e2
        ExpConcat         _    e1 e2       -> evalExpConcat e1 e2 

        ExpAdd            _    e1 e2       -> evalIntsToInt   (+)     e1 e2 noCheck
        ExpSub            _    e1 e2       -> evalIntsToInt   (-)     e1 e2 noCheck
        ExpMultiply       _    e1 e2       -> evalIntsToInt   (*)     e1 e2 noCheck
        ExpDivide     position e1 e2       -> evalExpDivide position  e1 e2
        ExpModulo     position e1 e2       -> evalExpModulo position  e1 e2
        ExpPower      position e1 e2       -> evalExpPower position   e1 e2

        ExpTrue                _           -> return $ BoolValue True
        ExpFalse               _           -> return $ BoolValue False
        ExpInteger             _ int       -> return $ IntValue  int
        ExpNegativeInteger     _ int       -> return $ IntValue $ negate int
        ExpFunctionApplication _ func xs   -> evalExpFunctionApplication func xs
        ExpLambdaApplication   _ lam xs    -> evalExpLambdaApplication lam xs
        ExpConsComma           _ xs        -> evalExpConsComma xs
        ExpConsDots            _ x xs      -> evalExpConsDots x xs
        ExpConsRangeClosed     _ start end -> evalExpConsRangeClosed start end
        ExpConsRangeOpen       _ start     -> evalExpConsRangeOpen start
