# Caesar Cipher

This program encoding and decoding text files according to the
method of Caesar cipher.

The program can execute three commands:
* **encode** - encode a source file.
* **decode** - decode a source file.
* **check** - the program will take one source file and an encrypted
file and will check if the first one is a valid encryption of the
second one and will return the parameter of the encryption.

If the user chose encode or decode the program should get 4
arguments through the CLI:
- command <encode|decode> - the command to execute.
- parameter k - number of indentations.
- source file path.
- output file path.

If the user chose check command the program should get 3 arguments
- command - "check".
- source file path.
- encrypted file path.
