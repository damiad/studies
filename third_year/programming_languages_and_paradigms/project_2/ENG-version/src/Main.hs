module Main where

import Built.ParOSkell
import Evaluation (evaluate)
import Err
import System.Environment (getArgs)
import System.IO
import TypeInference (inferTypeOfExpression)

interpreterUsage :: String
interpreterUsage = unlines 
    [ "Usage: ./interpreter <file>        % Interprets a file"
    , "   or: ./interpreter               % Interprets standard input"
    ]

processInput :: String -> Either String String
processInput input = 
    let parsedProg = myLexer input 
    in case pExp parsedProg of
        Left lexErr -> Left $ "Lexical Error: " ++ lexErr
        Right program -> case inferTypeOfExpression program of
            Left typeErr -> Left $ formatError "Type Error" typeErr
            Right progType -> case evaluate program of
                Left evalErr -> Left $ formatError "Runtime Error" evalErr
                Right result -> Right $ show result ++ " :: " ++ show progType

printResult :: Either String String -> IO ()
printResult (Left str) = putStrLn str
printResult (Right str) = hPutStrLn stderr str

printInterpreterUsage :: IO ()
printInterpreterUsage = putStrLn interpreterUsage

processInputAndPrintResultFromStdin :: IO ()
processInputAndPrintResultFromStdin = do
    input <- getContents
    let output = processInput input
    printResult output

processInputAndPrintResultFromFile :: FilePath -> IO ()
processInputAndPrintResultFromFile filename = do
    input <- readFile filename
    let output = processInput input
    printResult output

printInvalidArgumentsError :: IO ()
printInvalidArgumentsError = do
    hPutStrLn stderr "Invalid number of arguments (use -h or --help for help)"
    hPutStrLn stderr interpreterUsage

main :: IO ()
main = do
    arguments <- getArgs
    case arguments of
        ["-h"]     -> printInterpreterUsage
        ["--help"] -> printInterpreterUsage
        []        -> processInputAndPrintResultFromStdin
        [file]    -> processInputAndPrintResultFromFile file
        _         -> printInvalidArgumentsError
