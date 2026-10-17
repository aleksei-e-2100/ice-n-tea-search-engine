<p align="right">
  <a href="./README.md">Русский</a> | <strong>English</strong> 
</p>

# iCE && tEA Search Engine

## Annotation
iCE && tEA Search Engine is a search engine designed for a corporate portal. It is intended for fast information retrieval across a large number of text documents and can be used either as a standalone application or as a component within a website, web service, or any other software system. Any frontend or external application can interact with the engine by providing search queries in JSON file and reading search results from JSON output file.

### The engine operates according to the following principles:
1. Before launch, the configuration file `config.json` specifies the list of files in which the engine will perform the search.
2. The engine automatically scans the specified files and builds an index that allows it to find the most relevant documents for any search request. During indexing, words are tokenized: punctuation marks at the beginning and end of a word are removed.
3. Search requests are provided through the JSON file `requests.json`. A request is a set of words used to search within the documents.
4. The request is tokenized in the same way as the text documents. The engine then finds documents that contain the specified words.
5. Search results are ranked, sorted, and written to the JSON file `answers.json`. Any application can read this file and use the results in its interface.


## Using the engine

The files `config.json` and `requests.json` must be located in the same directory as the engine’s executable. Without them, the engine cannot start. Their contents are validated at startup.

### In `config.json`:
- The file must contain the required keys "config" with engine settings and "files" with the list of files to be searched.
- The file must specify a configuration version ("version"). The engine will not start if the version is missing or does not match the engine’s version.
- Optional parameter "max_responses"  (the maximum number of responses per request). If "max_responses"" is not specified, the engine returns the five most relevant documents by default.
- The list of files must be provided as text strings. If a file is missing or cannot be opened, the engine will report an error.

### In `requests.json`:
- The file must contain the search requests that the engine will process.


## Examples of configuration and search results
Sample `config.json` and `requests.json` files are located in the `release` directory.  
Sample text files for searching are located in `release/text_files`.

### `answers.json`:
A sample `answers.json` file is also provided in the `release` directory.
- The key "request00X" corresponds to the index of the request (matching the order in `requests.json`).
- The key "result" is true if at least one document was found, otherwise false. Empty requests also produce false.
- The key "doc_id" indicates the index of the document in which the result was found (matching the order of files in `config.json`, starting from zero).
- The key "rank" indicates the document’s relevance. If multiple documents match the request, they are listed in descending order of relevance.


## Limitations
The engine has the following limitations, all of which are dictated by the goals for which it was created:
- It works only with plain text files (for example, .txt). It does not search inside Word documents or other binary formats.
- The engine is case‑sensitive: words must match the case used in the request.
- During tokenization, punctuation is removed only at the beginning and end of a word. Punctuation inside a word is treated as part of the word.
- From the engine’s perspective, words in different grammatical forms (cases, declensions, tenses, etc.) are considered different words. For example, "house" and "houses" are treated as distinct tokens and are not matched to each other. The engine does not perform lemmatization, stemming, or morphological analysis.

## Technology stack
- The engine uses C++23 and STL containers.
- File indexing is performed in multiple threads. File reading is always sequential (single‑threaded) to avoid issues with older HDDs. A thread pool is implemented. After reading a file from disk, the engine hands it off to the thread pool for indexing. To minimize memory usage and allow searching across a large number of files, the file contents are not stored in memory after indexing is complete.

External libraries:
- JSON for Modern C++ for JSON processing.
- GoogleTest for unit testing.


## Build
The project uses CMake.
Supported compilers:
Windows: MSVC, MinGW
Linux: gcc (g++), clang
An internet connection is required during the initial CMake run, because JSON for Modern C++ and GoogleTest will be downloaded automatically.

### Example build procedure:
Clone the repository.  
Move into the project directory.  

Run the following commands:  

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

The resulting binary will be located in the release directory.

## TODO
- Add remote API layer (JSON‑RPC or REST) as an optional extension for external integrations.
