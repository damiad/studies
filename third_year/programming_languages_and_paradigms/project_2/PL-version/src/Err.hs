module Err
  ( Err(..)
  , Position
  , ErrorMessage
  , formatError
  ) where
import Built.AbsOSkell(BNFC'Position)
import Control.Monad(MonadPlus(mplus))

-- Type synonyms for clarity
type Position  = BNFC'Position
type ErrorMessage  = String

newtype Err = Err (Position, ErrorMessage) 

-- Combines two errors, appending their messages
instance Semigroup Err where
  Err (posA, msgA) <> Err (posB, msgB) = Err (posA `mplus` posB, msgA <> msgB)

instance Monoid Err where
  mempty  = Err (Nothing, "")
  mappend = (<>)

formatError :: ErrorMessage -> Err -> String
formatError errorType (Err (position, errorMsg)) =
  "<interpreter>:" ++ positionTxt ++ errorType ++ " - " ++ errorMsg
  where
    positionTxt = case position of
      Nothing        -> " "
      Just (row, column) -> "(" ++ show row ++ ":" ++ show column ++ "): "
