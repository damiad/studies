{-# LANGUAGE FlexibleContexts #-}
{-# LANGUAGE TypeSynonymInstances #-}
{-# LANGUAGE FlexibleInstances #-}
{-# LANGUAGE LambdaCase #-}

module TypeInference (inferTypeOfExpression) where

import Built.AbsOSkell
import Err

import Control.Monad.Trans (lift)
import Control.Monad.Except (ExceptT, runExceptT, throwError)
import Control.Monad.Reader (ReaderT, runReaderT, local, ask)
import Control.Monad.State (StateT, runStateT, put, get)
import Control.Applicative (Alternative((<|>)))
import Data.Maybe (fromMaybe)
import Control.Monad.Identity

import qualified Data.Set as Set
import qualified Data.Map as Map
import Data.Set (Set)
import Data.Map (Map)

-- Type definition
infixr :>>
data Type = TypeBool
          | TypeInt
          | TypeList Type
          | TypeVar  TypeVar 
          | Type :>> Type
          deriving (Eq)

-- Show instance for Type
instance Show Type where
    showsPrec _ TypeBool          = showString "Bool"
    showsPrec _ TypeInt           = showString "Int"
    showsPrec _ (TypeList t)      = showString "[" . shows t . showString "]"
    showsPrec _ (TypeVar var)     = showString "t" . shows var
    showsPrec d (left :>> right)  =
        showParen (d > arrowPrecLeft) $
            showsPrec arrowPrecLeft left .
            showString " -> " .
            showsPrec (arrowPrecRight + 1) right
        where
            arrowPrecLeft  = 9
            arrowPrecRight = 8

-- Qualified type definition
data QualType = ForAll VariableSet Type
    deriving Show

type VariableSet = Set TypeVar
type Environment = Map Ident QualType
type TypeVar     = Int
type TypeMap     = Map TypeVar Type
type InM a       = ExceptT Err (StateT (TypeVar, TypeMap) (ReaderT Environment Identity)) a

-- Standard library functions
stdlib :: Map Ident QualType
stdlib = Map.fromList [
    (Ident "head", 
        ForAll (Set.singleton (-1)) $ TypeList (TypeVar (-1)) :>> TypeVar (-1)),
    (Ident "tail", 
        ForAll (Set.singleton (-2)) $ TypeList (TypeVar (-2)) :>> TypeList (TypeVar (-2))),
    (Ident "null", 
        ForAll (Set.singleton (-3)) $ TypeList (TypeVar (-3)) :>> TypeBool)
  ]

-- Run the InM monad
runInM :: InM a -> (TypeVar, TypeMap) -> Environment -> (Either Err a, (TypeVar, TypeMap))
runInM im state env = runIdentity $ runReaderT (runStateT (runExceptT im) state) env

-- Infer the type of an expression
inferTypeOfExpression :: Exp -> Either Err Type
inferTypeOfExpression e = case (runInM (typeOf e) (0, Map.empty) stdlib) of
    (Left err, _)        -> Left err
    (Right t, (_, substitutions)) -> return $ applySubstitution substitutions t

-- Generate a new type variable
newTypeVar :: InM Type
newTypeVar = do
    (currentVar, typeMap) <- lift get
    lift $ put (nextVariable currentVar, typeMap)
    return $ TypeVar currentVar
  where
    nextVariable = (+1)

-- Check for the presence of a type variable in a type
hasVariable :: TypeVar -> Type -> InM Bool
hasVariable var (leftType :>> rightType) = hasVariable var leftType <|> hasVariable var rightType
hasVariable var (TypeVar var')           = return $ var == var'
hasVariable var (TypeList elementType)   = hasVariable var elementType
hasVariable _ _                          = return False

-- Apply a substitution to a type
applySubstitution :: TypeMap -> Type -> Type
applySubstitution substitutions (leftType :>> rightType) =
    applySubstitution substitutions leftType :>> applySubstitution substitutions rightType
applySubstitution substitutions (TypeVar var) =
    fromMaybe (TypeVar var) $ Map.lookup var substitutions
applySubstitution substitutions (TypeList elementType) =
    TypeList $ applySubstitution substitutions elementType
applySubstitution _ otherType = otherType

-- Set a substitution
setSubstitution :: TypeVar -> Type -> InM ()
setSubstitution var t = do
    (_, currentSubs) <- lift get
    let updatedSubs = Map.insert var t currentSubs
        appliedSubs = Map.map (applySubstitution updatedSubs) updatedSubs
    (currentVar, _) <- lift get
    lift $ put (currentVar, appliedSubs)

-- Apply substitutions in the monad
applySubstitutionsM :: Type -> InM Type
applySubstitutionsM typeToSubstitute = do
    (_, substitutions) <- lift get
    return $ applySubstitution substitutions typeToSubstitute

-- Unify two types
unifyTypes :: Position -> Type -> Type -> InM ()
unifyTypes position type1 type2 = do
    type1' <- applySubstitutionsM type1
    type2' <- applySubstitutionsM type2
    unifyRecursively type1' type2'
  where
    unifyRecursively :: Type -> Type -> InM ()
    unifyRecursively TypeBool TypeBool                         = return ()
    unifyRecursively TypeInt TypeInt                           = return ()
    unifyRecursively (TypeList innerType1) (TypeList innerType2) =
        unifyRecursively innerType1 innerType2
    unifyRecursively (left1 :>> right1) (left2 :>> right2) = do
        unifyTypes position left1 left2
        unifyTypes position right1 right2
    unifyRecursively (TypeVar var1) (TypeVar var2) =
        if var1 == var2
        then return ()
        else do
            substituteVar1 <- applySubstitutionsM (TypeVar var1)
            substituteVar2 <- applySubstitutionsM (TypeVar var2)
            case (substituteVar1, substituteVar2) of
                (TypeVar _, TypeVar _) -> setSubstitution var1 (TypeVar var2)
                _ -> unifyTypes position substituteVar1 substituteVar2
    unifyRecursively (TypeVar var) typeToUnify = do
        isRecursive <- hasVariable var typeToUnify
        if isRecursive
        then throwError $ Err (position, "Type unification failed. Recursive type detected: " ++ show typeToUnify)
        else do
            typeToUnify' <- applySubstitutionsM typeToUnify
            setSubstitution var typeToUnify'
    unifyRecursively typeToUnify (TypeVar var) = unifyRecursively (TypeVar var) typeToUnify
    unifyRecursively type1' type2' =
        throwError $ Err (position, "Type unification failed. Incompatible types: " ++ show type1' ++ " and " ++ show type2')

-- Generalize a type
generalize :: Type -> InM QualType
generalize t = do
    tVars <- freeTypeVariables t
    env <- ask
    envVars <- freeVarsForEnvironment env
    let quantifiedVars = Set.difference tVars envVars
    return $ ForAll quantifiedVars t
  where
    freeTypeVariables ty = do
        ty' <- applySubstitutionsM ty
        return $ extractVars ty'
      where 
        extractVars typ = case typ of 
            TypeVar var      -> Set.singleton var
            (left :>> right) -> Set.union (extractVars left) (extractVars right)
            (TypeList inner) -> extractVars inner
            _                -> Set.empty
    freeVarsForEnvironment env = do
        foldM collectVars Set.empty (Map.elems env) 
      where
        collectVars acc (ForAll vars ty) = do
            freeVars <- freeTypeVariables ty
            return $ Set.union acc $ Set.difference freeVars vars

-- Infer the type of a parameter
typeOfPar :: Par -> InM Type
typeOfPar par = case par of
    ParTrue _                              -> return $ TypeBool
    ParFalse _                             -> return $ TypeBool
    ParInteger _ _                         -> return $ TypeInt
    ParFunction position f                 -> typeOf $ ExpFunctionApplication position f []
    ParLambda position f                   -> typeOf $ ExpLambdaApplication position f []
    ParConsComma position xs               -> typeOf $ ExpConsComma position xs
    ParConsRangeClosed position left right -> typeOf $ ExpConsRangeClosed position left right
    ParConsRangeOpen position left         -> typeOf $ ExpConsRangeOpen position left

-- Infer the type for a binary operation
typeOfBinaryOperation :: Position -> Exp -> Exp -> Type -> Type -> InM Type
typeOfBinaryOperation position exp1 exp2 inputType outputType = do
    type1 <- typeOf exp1
    type2 <- typeOf exp2
    unifyTypes position type1 inputType
    unifyTypes position type2 inputType
    return outputType

-- Instantiate a qualified type
instantiate :: QualType -> InM Type
instantiate (ForAll vars ty) = do
    ty' <- applySubstitutionsM ty
    substitutions <- sequence $ Map.fromSet (const newTypeVar) vars
    return $ applySubstitution substitutions ty'

-- Infer the type of an if expression
inferExpIf :: Position -> Exp -> Exp -> Exp -> InM Type
inferExpIf position cond whenTrue whenFalse = do
    condT <- typeOf cond
    onTrueT <- typeOf whenTrue
    onFalseT <- typeOf whenFalse
    unifyTypes position condT TypeBool
    unifyTypes position onTrueT onFalseT
    return onTrueT

-- Infer the type of a let expression
inferExpLet :: Position -> Ident -> [Ident] -> Exp -> Exp -> InM Type
inferExpLet position x xs body e = do
    xType <- newTypeVar
    bodyType <- local (Map.insert x (ForAll Set.empty xType)) $ typeOf $ ExpLambda position xs body
    unifyTypes position xType bodyType
    xType' <- generalize xType
    local (Map.insert x xType') $ typeOf e

-- Infer the type of a lambda expression
inferExpLambda :: Position -> [Ident] -> Exp -> InM Type
inferExpLambda _ [] e = typeOf e
inferExpLambda position (x:xs) e = do
    xType <- newTypeVar
    xsType <- local (Map.insert x (ForAll Set.empty xType)) (typeOf $ ExpLambda position xs e)
    applySubstitutionsM $ xType :>> xsType

-- Infer the type of list concatenation
inferExpConcat :: Position -> Exp -> Exp -> InM Type
inferExpConcat position exp1 exp2 = do
    type1 <- typeOf exp1
    type2 <- typeOf exp2
    var <- newTypeVar
    unifyTypes position type1 $ TypeList var
    unifyTypes position type2 $ TypeList var
    return $ TypeList var

-- Infer the type for function application
inferApplicationType :: Position -> Type -> [Par] -> InM Type
inferApplicationType position fun args = foldM inferType fun args
  where
    inferType :: Type -> Par -> InM Type
    inferType acc arg = do
        argType <- typeOfPar arg
        var <- newTypeVar
        unifyTypes position acc $ argType :>> var
        return var

-- Infer the type of a function application
inferExpFunctionApplication :: Position -> Ident -> [Par] -> InM Type
inferExpFunctionApplication position name xs = do
    environment <- ask
    case Map.lookup name environment of
        Nothing -> 
            let Ident nameStr = name
            in throwError $ Err (position, "Variable not in scope: " ++ nameStr)
        Just functionType -> do
            instantiatedFunctionType <- instantiate functionType
            inferApplicationType position instantiatedFunctionType xs

-- Infer the type of a lambda application
inferExpLambdaApplication :: Position -> Exp -> [Par] -> InM Type
inferExpLambdaApplication position lambda xs = do
    lambdaType <- typeOf lambda
    inferApplicationType position lambdaType xs 

-- Infer the type of a list construction with commas
inferExpConsComma :: Position -> [Exp] -> InM Type
inferExpConsComma position expressions = do
    elementTypeVar <- newTypeVar
    foldM inferAndUnify (TypeList elementTypeVar) expressions
  where
    inferAndUnify acc expr = do
        expType <- typeOf expr
        unifyTypes position acc $ TypeList expType
        return acc

-- Infer the type of a list construction with dots
inferConsDots :: Position -> Par -> Par -> InM Type
inferConsDots position firstPar restPar = do
    firstParType <- typeOfPar firstPar
    restParType <- typeOfPar restPar
    var <- newTypeVar
    unifyTypes position restParType $ TypeList var
    unifyTypes position restParType $ TypeList firstParType
    return $ TypeList firstParType

-- Infer the type of a closed range
inferExpConsRangeClosed :: Position -> Par -> Par -> InM Type
inferExpConsRangeClosed position startPar endPar = do
    startParType <- typeOfPar startPar
    endParType <- typeOfPar endPar
    unifyTypes position startParType TypeInt
    unifyTypes position endParType TypeInt
    return $ TypeList TypeInt

-- Main function to infer the type of an expression
typeOf :: Exp -> InM Type
typeOf expr = do
    case expr of 
        ExpIf position cond whenTrue whenFalse   -> inferExpIf position cond whenTrue whenFalse
        ExpLet position x xs body e              -> inferExpLet position x xs body e
        ExpLambda position xs e                  -> inferExpLambda position xs e

        ExpAnd position e1 e2                    -> typeOfBinaryOperation position e1 e2 TypeBool TypeBool
        ExpOr position e1 e2                     -> typeOfBinaryOperation position e1 e2 TypeBool TypeBool
        ExpNot position e                        -> typeOfBinaryOperation position e e TypeBool TypeBool
        ExpEqual position e1 e2                  -> typeOfBinaryOperation position e1 e2 TypeInt TypeBool
        ExpNotEqual position e1 e2               -> typeOfBinaryOperation position e1 e2 TypeInt TypeBool
        ExpLessThan position e1 e2               -> typeOfBinaryOperation position e1 e2 TypeInt TypeBool
        ExpGreaterThan position e1 e2            -> typeOfBinaryOperation position e1 e2 TypeInt TypeBool
        ExpLessEqual position e1 e2              -> typeOfBinaryOperation position e1 e2 TypeInt TypeBool
        ExpGreaterEqual position e1 e2           -> typeOfBinaryOperation position e1 e2 TypeInt TypeBool
        ExpConcat position e1 e2                 -> inferExpConcat position e1 e2

        ExpAdd position e1 e2                    -> typeOfBinaryOperation position e1 e2 TypeInt TypeInt
        ExpSub position e1 e2                    -> typeOfBinaryOperation position e1 e2 TypeInt TypeInt
        ExpMultiply position e1 e2               -> typeOfBinaryOperation position e1 e2 TypeInt TypeInt
        ExpDivide position e1 e2                 -> typeOfBinaryOperation position e1 e2 TypeInt TypeInt
        ExpModulo position e1 e2                 -> typeOfBinaryOperation position e1 e2 TypeInt TypeInt
        ExpPower position e1 e2                  -> typeOfBinaryOperation position e1 e2 TypeInt TypeInt

        ExpTrue _                                -> return TypeBool
        ExpFalse _                               -> return TypeBool
        ExpInteger _ _                           -> return TypeInt
        ExpNegativeInteger _ _                   -> return TypeInt
        ExpFunctionApplication position fun xs   -> inferExpFunctionApplication position fun xs
        ExpLambdaApplication position lam xs     -> inferExpLambdaApplication position lam xs
        ExpConsComma position xs                 -> inferExpConsComma position xs
        ExpConsDots position x xs                -> inferConsDots position x xs
        ExpConsRangeClosed position start end    -> inferExpConsRangeClosed position start end
        ExpConsRangeOpen position start          -> typeOf $ ExpConsRangeClosed position start start
