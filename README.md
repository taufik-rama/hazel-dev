# hazel-dev

Reimplementation of [Hazel](https://github.com/TheCherno/Hazel)

## How to's

Clone via: `$ git clone --recursive https://github.com/taufik-rama/hazel-dev`

I `source` some variables on `script/variables.sh` for convenience (make sure to change the path to your absolute path)

  - `$ source script/variables.sh`

### Building

Create the required directories

  - `mkdir hazel-dev/build sandbox/build`

#### `vscode` tasks

- `ctrl + shift + p` -> `Tasks: Run Task` -> `Build all` -> Choose output scanner (optional, pick any)

#### Command line

Some alias are defined on `variables.sh`

  - ```
    $ cddev
    $ cmake ..
    $ ninja # or `make`, depending on your generator
    ```

  - ```
    $ cdsandbox
    $ cmake ..
    $ ninja # or `make`, depending on your generator
    ```

### Running

Just run the `sandbox` app

  - `$ cdsandbox && ./sandbox`
