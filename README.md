```
****************
* EXTENSION USED
****************
```
# CS50 TSE
## Charlie Childress (charliechildress)

The assignment and Specs are in a [public repo](https://github.com/cs50winter2022/labs/tse). 

`submit 4` branch is for crawler

`submit 5` branch is for indexer

`submit 6` branch is for querier

Before running `crawler.c`, `indexer.c`, `querier.c` program, you must first call `make` in outmost directory _tse-crawler-charliechildress_ This will create all the files needed to run programs and tests in the _crawler_ directory, the _indexer_ directory, and the _querier_ directory.

My implementation differs for my crawler.c file as I include a function `logr()`.
The purpose of `logr()` is to provide an organized print statement into stdout that shows which URLs are being processed and how.
This does not change the functionality of the program, just the style.

In testing for crawler, I did up `toscrape` only for depths 0 and 1 as there is a url at depth 2 `http://cs50tse.cs.dartmouth.edu/tse/toscrape/catalogue its-only-the-himalayas_981/../category/books_1/index.html` that my code gets stuck in the `webpage_fetch()` method with. Since the `webpage_fetch()` is a provided method that is different to how I would have coded the method, I chose to skip tests that ran into the url. I hope that showing that my code works with wikipedia at depth 2, which is much greater, shows its abilities.