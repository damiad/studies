### Overview

This project involves implementing sets and graphs in Haskell, adhering to specific requirements and operations. The goal is to define and test the necessary operations for sets and graphs using custom data structures and ensure that they meet given properties and laws.

### A. Sets

We implement sets using the following type and operations:

#### Data Type
```haskell
data Set a = Empty
           | Singleton a
           | Union (Set a) (Set a)
```

#### Operations
```haskell
empty     :: Set a
null      :: Set a -> Bool
member    :: Eq a => a -> Set a -> Bool
singleton :: a -> Set a
fromList  :: [a] -> Set a
toList    :: Set a -> [a]
toAscList :: Ord a => Set a -> [a]
union     :: Set a -> Set a -> Set a
insert    :: a -> Set a -> Set a
```

#### Instances
```haskell
instance Ord a => Eq (Set a) where
instance Semigroup (Set a) where
instance Monoid (Set a) where
instance Show a => Show (Set a) where
instance Functor Set where
```

#### Properties
- `leftUnit :: Set Int -> Bool`
- `rightUnit :: Set Int -> Bool`
- `unionCommutative :: Set Int -> Set Int -> Bool`
- `unionIdempotent :: Set Int -> Set Int -> Bool`
- `assoc :: Set Int -> Set Int -> Set Int -> Bool`

### B. Graphs

We define directed graphs using two different representations: `Relation` and `Basic`.

#### Data Types
```haskell
data Relation a = Relation { domain :: Set a, relation :: Set (a, a) }
    deriving (Eq, Show)

data Basic a = Empty
             | Vertex a
             | Union (Basic a) (Basic a)
             | Connect (Basic a) (Basic a)
```

#### Operations
```haskell
class Graph g where
  empty   :: g a
  vertex  :: a -> g a
  union   :: g a -> g a -> g a
  connect :: g a -> g a -> g a
```

#### Instances
```haskell
instance Graph Relation where
instance Graph Basic where
instance Ord a => Eq (Basic a) where
instance Functor Basic
```

#### Properties
- `leftUnit :: Basic Int -> Bool`
- `rightUnit :: Basic Int -> Bool`
- `connectAssociative :: Basic Int -> Basic Int -> Basic Int -> Bool`
- `unionCommutative :: Basic Int -> Basic Int -> Bool`
- `unionAssociative :: Basic Int -> Basic Int -> Basic Int -> Bool`
- `unionIdempotent :: Basic Int -> Basic Int -> Bool`
- `distributive :: Basic Int -> Basic Int -> Basic Int -> Bool`
- `decomposable :: Basic Int -> Basic Int -> Basic Int -> Bool`

#### Num Instance for Basic
```haskell
instance (Ord a, Num a) => Num (Basic a) where
    fromInteger = vertex . fromInteger
    (+)         = union
    (*)         = connect
    signum      = const empty
    abs         = id
    negate      = id
```

### Additional Functions

#### `mergeV`
Merges two vertices into one in a graph.
```haskell
mergeV :: Eq a => a -> a -> a -> Basic a -> Basic a
```

#### `todot`
Generates GraphViz representation of a graph.
```haskell
todot :: (Ord a, Show a) => Basic a -> String
```

#### `splitV`
Splits a vertex into two vertices in a graph.
```haskell
splitV :: Eq a => a -> a -> a -> Basic a -> Basic a
```

### Example Usage
```haskell
example34 :: Basic Int
example34 = 1 * 2 + 2 * (3 + 4) + (3 + 4) * 5 + 17

main :: IO ()
main = do
    putStrLn $ show example34
    putStrLn $ todot example34
```

### GraphViz Example Output
```plaintext
digraph {
1 -> 2;
2 -> 3;
2 -> 4;
3 -> 5;
4 -> 5;
17;
}
```

### Testing
Make sure all properties and tests pass efficiently. The provided implementations should complete within a reasonable time frame (~2 seconds).

### File Structure
- `Set.hs` - Implementation of sets and related operations.
- `Graph.hs` - Implementation of graphs and related operations.
- `example34.dot` - Example GraphViz output for visual representation.

### Requirements
- Haskell (base package only)
- No external libraries such as `Data.Set`

Ensure the implementation is efficient and adheres to the provided specifications.