# reanalyze

Experimental analyses: for dead values/types, and for termination.


**Status `master (v3.*)`:** [![Build
Status](https://dev.azure.com/ccrisccris/reanalyze/_apis/build/status/cristianoc.reanalyze?branchName=master)](https://dev.azure.com/ccrisccris/reanalyze/_build/latest?definitionId=1&branchName=master)


## Build
```sh
npm install
npm run build
npx esy x which reanalyze.exe
  /Users/cristianoc/reasonml/reanalyze/_esy/default/store/i/reanalyze-1a0dcc57/bin/reanalyze.exe
```

## Single File Test
```sh
echo "let unused = 34" > test.ml
ocamlc -c -bin-annot test.ml
reanalyze.exe -dce-cmt ./test.cmt
  Warning Dead Value
  test.ml 1:1-15
  unused is never used
  <-- line 1
  let unused = 34 [@@dead "unused"] 
```

## Single Directory Test
```sh
mkdir test
echo "let unused = 34 let used = 42" > test/test.ml
echo "let _ = Test.used" > test/use.ml
cd test
ocamlc -c -bin-annot *.ml
reanalyze.exe -dce-cmt .
  Warning Dead Value
  test.ml 1:1-15
  unused is never used
  <-- line 1
  let unused = 34 [@@dead "unused"]  let used = 42
  Warning Dead Value
  use.ml 1:1-17
  _ has no side effects and can be removed
  <-- line 1
  let _ = Test.used [@@dead "_"] 
```
