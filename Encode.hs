module Main where

import           Control.Applicative        ((<$>))
import           Data.Binary.IEEE754        (putFloat32le, putFloat32be)
import           Data.Binary.Put            (Put, putWord32be, putWord64be, putWord32le, putWord64le, runPut)
import qualified Data.ByteString.Lazy.Char8 as BS
import           Data.List                  (genericLength)
import           System.Environment         (getArgs)
import           System.IO                  (hPutStrLn, stderr)

data Endianness = LittleEndian | BigEndian
data WordSize   = ThirtyTwoBit | SixtyFourBit

main :: IO ()
main = do
    args <- getArgs
    if length args /= 3
    then printUsage
    else packFile (toEndianness $ args !! 0) (toWordSize $ args !! 1) (args !! 2)

printUsage :: IO ()
printUsage = hPutStrLn stderr "Usage: ./Encode --little|--big --32|--64 filename"

toEndianness :: String -> Endianness
toEndianness "--little" = LittleEndian
toEndianness "--big"    = BigEndian
toEndianness _          = error "first arg: --little or --big"

toWordSize :: String -> WordSize
toWordSize "--32" = ThirtyTwoBit
toWordSize "--64" = SixtyFourBit
toWordSize _      = error "second arg: --32 or --64"

packFile :: Endianness -> WordSize -> FilePath -> IO ()
packFile e ws path = map (read . BS.unpack) . BS.words <$> BS.readFile path >>= encode e ws (path ++ "-packed")

encode :: Endianness -> WordSize -> FilePath -> [Float] -> IO ()
encode e ws path = BS.writeFile path . runPut . putFloats e ws

putFloats :: Endianness -> WordSize -> [Float] -> Put
putFloats LittleEndian ThirtyTwoBit = putFloats' putWord32le putFloat32le
putFloats LittleEndian SixtyFourBit = putFloats' putWord64le putFloat32le
putFloats BigEndian    ThirtyTwoBit = putFloats' putWord32be putFloat32be
putFloats BigEndian    SixtyFourBit = putFloats' putWord64be putFloat32be

putFloats' :: Num a => (a -> Put) -> (Float -> Put) -> [Float] -> Put
putFloats' f g xs = f (genericLength xs) >> mapM_ g xs

{-decode :: FilePath -> IO [Float]-}
{-decode path = runGet getFloats <$> BS.readFile path-}

{-getFloats :: Get [Float]-}
{-getFloats = getWord32le >>= \len -> replicateM (fromIntegral len) getFloat32le-}
