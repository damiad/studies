module Graph where
import Set(Set)
import qualified Set as Set
class Graph g where
  empty   :: g a
  vertex  :: a -> g a
  union   :: g a -> g a -> g a
  connect :: g a -> g a -> g a

data Relation a = Relation { domain :: Set a, relation :: Set (a, a) }
    deriving (Eq, Show)

data Basic a = Empty
             | Vertex a
             | Union (Basic a) (Basic a)
             | Connect (Basic a) (Basic a)

instance Graph Relation where
  empty = Relation Set.empty Set.empty
  vertex x = Relation (Set.singleton x) Set.empty
  union (Relation dom1 re1) (Relation dom2 re2) = Relation (Set.union dom1 dom2) (Set.union re1 re2)
  connect (Relation dom1 re1) (Relation dom2 re2) = Relation domSet reSet
    where
      crossEdges = Set.fromList [(x, y) | x <- Set.toList dom1, y <- Set.toList dom2]
      domSet = Set.union dom1 dom2
      reSet = Set.union (Set.union re1 re2) crossEdges

instance (Ord a, Num a) => Num (Relation a) where
  fromInteger = vertex . fromInteger
  (Relation dom1 re1) + (Relation dom2 re2) = Relation domSet reSet
    where
      domSet = Set.fromList (Set.toAscList (Set.union dom1 dom2))
      reSet = Set.fromList (Set.toAscList (Set.union re1 re2))
  (Relation dom1 re1) * (Relation dom2 re2) = Relation domSet reSet
    where
      crossEdges = Set.fromList [(x, y) | x <- Set.toAscList dom1, y <- Set.toAscList dom2]
      domSet = Set.fromList (Set.toAscList (Set.union dom1 dom2))
      reSet = Set.fromList (Set.toAscList (Set.union (Set.union re1 re2) crossEdges))
  signum = const empty
  abs = id
  negate = id

instance Graph Basic where
  empty = Empty
  vertex x = Vertex x
  union g1 g2 = Union g1 g2
  connect g1 g2 = Connect g1 g2

instance (Ord a) => Eq (Basic a) where
  g1 == g2 = toVerticesEdges g1 == toVerticesEdges g2

toVerticesEdges :: (Ord a) => Basic a -> ([a], [(a, a)])
toVerticesEdges Empty = ([], [])
toVerticesEdges (Vertex x) = ([x], [])
toVerticesEdges (Union g1 g2) = (unionVertices, unionEdges)
  where
    (ver1, edg1) = toVerticesEdges g1
    (ver2, edg2) = toVerticesEdges g2
    unionVertices = unionSorted ver1 ver2
    unionEdges = unionSorted edg1 edg2
toVerticesEdges (Connect g1 g2) = (unionVertices, unionAllEdges)
  where
    (ver1, edg1) = toVerticesEdges g1
    (ver2, edg2) = toVerticesEdges g2
    unionVertices = unionSorted ver1 ver2
    crossEdges = [(x, y) | x <- ver1, y <- ver2]
    unionAllEdges = unionSorted crossEdges (unionSorted edg1 edg2)

unionSorted :: Ord a => [a] -> [a] -> [a]
unionSorted [] rightP = rightP
unionSorted leftP [] = leftP
unionSorted (l:leftP) (r:rightP)
  | l < r     = l : unionSorted leftP (r:rightP)
  | l > r     = r : unionSorted (l:leftP) rightP
  | otherwise = l : unionSorted leftP rightP

instance (Ord a, Num a) => Num (Basic a) where
    fromInteger = vertex . fromInteger
    (+)         = union
    (*)         = connect
    signum      = const empty
    abs         = id
    negate      = id

instance Semigroup (Basic a) where
  (<>) = union

instance Monoid (Basic a) where
  mempty = empty
  mappend = (<>)

fromBasic :: Graph g => Basic a -> g a
fromBasic Empty = empty
fromBasic (Vertex x) = vertex x
fromBasic (Union g1 g2) = union (fromBasic g1) (fromBasic g2)
fromBasic (Connect g1 g2) = connect (fromBasic g1) (fromBasic g2)

instance (Ord a, Show a) => Show (Basic a) where
  show graph = "edges " ++ show allEdges ++ " + vertices " ++ show verticesNoEdges
    where
      (allEdges, verticesNoEdges) = edgesAndSeparateVertices graph

edgesAndSeparateVertices :: Ord a => Basic a -> ([(a, a)], [a])
edgesAndSeparateVertices graph = (allEdges, verticesNoEdges)
  where
    (allVertices, allEdges) = toVerticesEdges graph
    verticesInEdges = unionSorted (map fst allEdges) (map snd allEdges)
    verticesNoEdges = subtractVertices allVertices verticesInEdges

subtractVertices :: Ord a => [a] -> [a] -> [a]
subtractVertices [] _ = []
subtractVertices xs [] = xs
subtractVertices (x:xs) (y:ys)
  | x < y = x : subtractVertices xs (y:ys)
  | x > y = subtractVertices (x:xs) ys
  | otherwise = subtractVertices xs ys

-- | Example graph
-- >>> example34
-- edges [(1,2),(2,3),(2,4),(3,5),(4,5)] + vertices [17]

example34 :: Basic Int
example34 = 1*2 + 2*(3+4) + (3+4)*5 + 17

todot :: (Ord a, Show a) => Basic a -> String
todot graph = "digraph {\n" ++ edgesToDot allEdges ++ verticesToDot verticesNoEdges ++ "}"
  where
    (allEdges, verticesNoEdges) = edgesAndSeparateVertices graph

verticesToDot :: Show a => [a] -> String
verticesToDot xs = unlines $ map (\v -> show v ++ ";") xs

edgesToDot :: (Show a) => [(a, a)] -> String
edgesToDot allEdges = unlines $ map (\(x, y) -> show x ++ " -> " ++ show y ++ ";") allEdges

instance Functor Basic where
  fmap _ Empty = Empty
  fmap f (Vertex x) = Vertex (f x)
  fmap f (Union g1 g2) = Union (fmap f g1) (fmap f g2)
  fmap f (Connect g1 g2) = Connect (fmap f g1) (fmap f g2)

-- | Merge vertices
-- >>> mergeV 3 4 34 example34
-- edges [(1,2),(2,34),(34,5)] + vertices [17]

mergeV :: Eq a => a -> a -> a -> Basic a -> Basic a
mergeV a b c = fmap replace
  where
    replace x
      | x == a || x == b = c
      | otherwise = x

instance Applicative Basic where
    pure = Vertex
    Empty <*> _ = Empty
    _ <*> Empty = Empty
    Vertex f <*> x = fmap f x
    Union f g <*> x = Union (f <*> x) (g <*> x)
    Connect f g <*> x = Connect (f <*> x) (g <*> x)

instance Monad Basic where
    return = pure
    Empty >>= _ = Empty
    Vertex x >>= f = f x
    Union g1 g2 >>= f = Union (g1 >>= f) (g2 >>= f)
    Connect g1 g2 >>= f = Connect (g1 >>= f) (g2 >>= f)

-- | Split Vertex
-- >>> splitV 34 3 4 (mergeV 3 4 34 example34)
-- edges [(1,2),(2,3),(2,4),(3,5),(4,5)] + vertices [17]

splitV :: Eq a => a -> a -> a -> Basic a -> Basic a
splitV oldVertex newCopy1 newCopy2 graph =
  graph >>= \ver ->
    if ver == oldVertex
      then Union (Vertex newCopy1) (Vertex newCopy2)
      else Vertex ver
