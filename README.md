# Alpha Compiler

Alpha Compiler is a cpp project, which given an input src file written in 'alpha' language, compiles it and generates an executable file. 


## Contributing

Vakras Vasileios  
Gregory Papas  
Soultatos Stefanos


## Usage

* To build the project use:
    ```bash
    mkdir build
    cd build
    cmake ..
    ```
    * To build a debug version of the parser tree traversal 
        ```bash
        make dlog
        ```
    * To build a release version for generating the symbol table output and the quad.txt file, at the /build directory, for the phase 3
        ```bash
        make phase3
        ```
    * To build a release version for generating the symbol table output (includes colored error/warning messages and temp values)
        ```bash
        make r_syntax_analysis
        ```
    * To build a release version for generating the intermediate code (includes colored error/warning messages)
        ```bash
        make r_intermediate_code
        ```
    * To build a debug version for generating the symbol table
    output
        ```bash
        make d_syntax_analysis
        ```
    * To build a debug version for generating the intermediate code 
        ```bash
        make d_intermediate_code
        ```

* <b>run</b>   
    * To compile a src file written in alpha language 
        ```bash
        ./[exec] [src_file]
        ```
     

