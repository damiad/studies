module Set(Set(..), empty, null, singleton, union, fromList
              , member, toList, toAscList, elems
              ) where
import Prelude hiding(null)

data Set a = Empty
           | Singleton a
           | Union (Set a) (Set a)

empty :: Set a
empty = Empty

null :: Set a -> Bool
null Empty = True
null (Singleton _) = False
null (Union x y) = null x && null y

member :: Eq a => a -> Set a -> Bool
member _ Empty = False
member x (Singleton y) = x == y
member x (Union leftPart rightPart) = member x leftPart || member x rightPart

singleton :: a -> Set a
singleton = Singleton

fromList :: [a] -> Set a
fromList [] = Empty
fromList (x:xs) = Union (Singleton x) (fromList xs)

toList :: Set a -> [a]
toList Empty = []
toList (Singleton x) = [x]
toList (Union leftPart rightPart) = toList leftPart ++ toList rightPart

toAscList :: Ord a => Set a -> [a]
toAscList = sort . toList 
    where 
        sort [] = []
        sort [x] = [x]
        sort xs = unionSorted (sort leftPart) (sort rightPart)
            where 
                (leftPart, rightPart) = splitAt (length xs `div` 2) xs
                unionSorted [] rightP = rightP
                unionSorted leftP [] = leftP
                unionSorted (l:leftP) (r:rightP)
                    | l < r     = l : unionSorted leftP (r:rightP)
                    | l > r     = r : unionSorted (l:leftP) rightP
                    | otherwise = l : unionSorted leftP rightP

elems :: Set a -> [a]
elems = toList

union :: Set a -> Set a -> Set a
union Empty s2 = s2
union s1 Empty = s1
union s1 s2 = Union s1 s2

insert :: a -> Set a -> Set a
insert x Empty = Singleton x
insert x s = Union (Singleton x) s

instance Ord a => Eq (Set a) where
    s1 == s2 = toAscList s1 == toAscList s2

instance Semigroup (Set a) where
    (<>) = union

instance Monoid (Set a) where
    mempty = empty
    mappend = (<>)

instance Show a => Show (Set a) where
    show s = show (toList s)

instance Functor Set where
    fmap _ Empty = Empty
    fmap f (Singleton x) = Singleton (f x)
    fmap f (Union leftPart rightPart) = Union (fmap f leftPart) (fmap f rightPart)
