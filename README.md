# lswap
This is a command line tool for fast translating language to language by copying and pasting data to the clipboard.
```
Arguments:

lswap run
      Run in background mode

lswap run --log
      Run in background mode with logging

lswap config <source_lang> <target_lang>
      Change the source and target languages in the configuration file
```
## Showcase
![Showcase](https://github.com/xastrix/lswap/blob/master/media/showcase.gif)
## Compiling
Clone the repository
```c
 git clone https://github.com/xastrix/lswap.git
```
Run ```lswap.sln``` (Visual Studio Solution File) and compile in Release\x86
### Requirements
* Visual Studio
* Windows SDK
## Dependencies
[curl](https://github.com/curl/curl)  
## License
Code protected by [MIT LICENSE](https://github.com/xastrix/lswap/blob/master/LICENSE)
