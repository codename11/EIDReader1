# An c++ program that reads from Serbian smart card reader, a Serbian id card.

### It uses winapi since it's windows program. It's been tested on win10 x64.

### It follows documentation of CelikApi, but doesn't display all of data. Too much typing.

### It was been made using oop snd can displays Serbian character i.e. diacritics.

### It also has function that creates image from binary data. And other one that creates `person.json` file.

### It also display text data in console window.

### Using in, for example in php, by using exec method, you can read text data from console, but must consider it's order.

### There is also a method that display image as string(text) of ASCII indices, so you'll need to do lookup into ASCII table to reconstruct image.

#### Url link to repo that uses it: https://github.com/codename11/EIDReader
