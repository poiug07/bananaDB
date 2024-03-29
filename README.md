# bananaDB :banana:

<p align="center">
    <img src="./assets/banana.png" width="120">
</p>

Simple key-value store as an exploration project. Linux focused development, and tools.

## Build and run:
1. Generate build config using CMake: `cmake -B build`
2. Build project: `cmake --build build`
3. Run executable: `./bin/bananaDB`

## Connecting to the server:
Run:  
`nc localhost <port_number>`

## Querying the server:
- `GET <key>`  
Gets value of a given key.
- `SET <key> <val>`  
Sets the value of a given key.
- `DEL <key>`  
Deletes a key.
- `GETALL`  
Gets all the keys.
- `GETSET <key> <default>`  
Tries to get the value given a key. If the key is not found, the value is set to `default`.
- `QUIT`  
Terminates the connection.

## Added some Make commands for convenience. 
* `generate_cmake` - to generate cmake build directory.
* `run` - to run the project executable.
* `test` - to run tests.

## TODO
* Add tests to check lifetime of object.
* decide what is ownership in our module
* Consider fuzz testing
* Add Clang Tidy or something similar for formatting and/or static checks
* We implemented wrong GetSet logic: what we might want is, replacing value, but returning original value instead.