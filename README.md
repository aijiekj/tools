# tools
基于quazip——Qt编写的压缩、解压工具

基本Quazip而编写的一个简单的压缩与解压工具，
其中Quazip是自己修改之后的版本,主要是加入了因压缩、解压大文件时会导致软件假死问题。

注意：
1.该工具有一个严重的Bug,当压缩文件时生成的目标文件在压缩目录时，程序就会进入死循环中,
解决的办法：在压缩或者解压时把目标文件可以先放到一个指定的临时文件中，当目录处理完成后再进行拷贝


2.zipfiles是自己很早之前参照Quazip中的Test转写的，主要是加入时间戳且主是为了windows下的文件,
如果想用于其他平台请修改使用相应平台下的方法来解决这个问题
