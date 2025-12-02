# Workshop01: writeups

## artefact01

### What password is used to access the dashboard?

Run `strings artefact01` to display strings inside of the artefact01.
Some reading and trial and error should lead to you find the password.
`umbryx-access`

### What developer email is listed by the dashboard?

Notice there is an additional command missing from the dashboard panel, `help`, either by more closely analysing the strings,
or by viewing inside of a reversing tool of their choice, and seeing the decompiled code or by viewing the initial array storing the ASCII
values of the email.
`dev@umbryx.corp`

## artefact02

### What is the hidden decoded token stored by the program?

Opening the binary in a static analysis tool is our first step, as nothing appears to be particularly interesting upon first running it.
This code is relatively short, so directly analysing the assembly is not unreasonable (with the assistance of the analysis tool).
Upon reaching the data section we see that there is a `XOR_KEY` (`0x1a`) and an encoded token `OWXHCB7I_Y7UJ7NUQ7]P[Q` (this may be in hex).
Using the XOR_KEY on each character of the string results in the answer.
`UMBRYX-SEC-OP-TOK-GJAK`

## artefact03

### What is the decode "relay key" hidden by the program?

After basic triaging of your choice (e.g. strings, objdump), you should find an alphabet of some kind `Z$M0!Vx-9B@1Xr#o2f4yqL3wNs8K5j7TgQ6h+RcpHEtiCUAdJeFbvGlmnkSPauWYzOD` and various node names (e.g. `node-east.umbryx.corp`). These node names are actually red herrings and can be 
completely ignored. We should instead study the program's behaviour statically. There are actually two solution paths.

#### Solution Path 1:
In our decompilation, we notice two "decode" functions, `::decode_stage_1` and `::decode_stage_2`. The first stage involves some bitshift operations
and XOR'ing. The second stage uses the aforementioned base85-like alphabet to decode. You should notice that a pointer to `relay_table` is passed into
`decode_stage_1`. Finding this table in your static analysis and reconstructing the decoding string (via a method of your choice, e.g. rewriting in Python)
reveals the answer.

### Solution Path 2:
The program checks for the existence of a particular environment variable. The name of the environment variable is hidden by the program, and is reconstructed
at runtime. This serves as an easier solution for students who are observant. Running `UMRBYX_DEBUG=1 ./artefact05` will reveal the answer.

`UMB-RELAY-KEY-943A`

## artefact04

### What is the bitcoin address being stored in the program?

Opening this binary in static analysis tools is quite futile - there seems to be a lot more optimization/obfuscation compared to the other challenges. Running the binary tells us the `MINER_KEY` is not set, but setting this (presumably an environment variable) doesn't affect the behavior of the program at all.

Since the program is probably checking environment variables, we can use a program such as `ltrace` to see exactly what environment variables are being gotten. By running `ltrace ./artefact03` we can see that the actual environment variable being fetched is `UBX_MINER_KEY`.

The usage of `ltrace` continues for a bit more, finding the correct file that the program checks for `./config_u8x`, and finding the correct header the program checks for `UMBRIX`. Doing all these steps correctly will give the bitcoin wallet address: `493p83kkfjhx0wlh`

You could also potentially jump to the part of the code which prints the bitcoin wallet address in a program such as gdb.

## artefact05

### Can you provide a valid license key for the program?

Running `strings` on the program, we see that this program mentions using `upx`. If we do some digging, we can see that this `upx` is a packer used for executables. Notably, we can also see that there is a way to "unpack" it back to less obfuscated code. Running `upx -d artefact04` gives us a much less obfuscated binary that we can now reverse with standard techniques.
