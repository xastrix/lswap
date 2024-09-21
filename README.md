[Logo](https://github.com/xastrix/lswap/blob/master/media/logo.png)  
This is a command line tool for fast translating language to language by copying and pasting data to the clipboard.
```
lswap run --log
      With logging (Displaying nontranslated and translated texts)

lswap config <source_lang> <target_lang>
      Change the source and target languages in the configuration file
```
## Showcase
![Showcase](https://github.com/xastrix/lswap/blob/master/media/showcase.gif)
## Compiling
Clone the repository
```c
 git clone --recursive https://github.com/xastrix/lswap.git
```
Launch ```build-vsXX.bat``` (Where XX is the version of your VS).  
After building, run ```lswap.sln``` (Visual Studio Solution File) and compile project by pressing ```CTRL + Shift + B```
### Requirements
* Visual Studio
* Windows SDK
* [premake5](https://github.com/premake/premake-core/releases)
## Dependencies
[curl](https://github.com/curl/curl)  
## License
Code protected by [MIT LICENSE](https://github.com/xastrix/lswap/blob/master/LICENSE)
