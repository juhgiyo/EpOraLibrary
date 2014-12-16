[![Gratipay][gratipay-image]][gratipay-url]
EpOraLibrary
=======================
#### Visual C++ Oracle OCI Wrapper Library ####


DESCRIPTION
-----------

As many people have experienced `Oracle OCCI Library`'s incompatibilities, and problems 
in many cases such as
- Unicode environment.
- Visual C++ Projects, that are linked with MFC Libraries.
- Visual C++ Projects linked with other multiple libraries.
- etc.

EpOraLibrary solves such issues, and makes it easier for Visual C++ developers 
to use Oracle Database Operation within their development projects. 
EpOraLibrary is a custom wrapper library of Oracle OCI Library under a [MIT License](http://opensource.org/licenses/mit-license.php).
It supports Visual C++ 2005/2008/2010/2012.
Source can be downloaded at [EpOraLibrary](http://github.com/juhgiyo/eporalibrary)


FEATURES
--------

* Supports the most of operations that Oracle OCCI Library supports.
* No more linking problems, stated above.
* Supports full ASCII/Unicode development environment.
* Similar operation convention used as Oracle OCCI Library
  - People can be easily adapted to EpOralibrary.
* Easy installation 
  - No need to install any Oracle installation for the development.
  - See Installation Section below for the detail.


Installation
------------

* Link `EpOraLibrary.lib` and EpOraLibrary Header folders in your project.
* Add `oci.dll`, `orannzsbb10.dll`, `oraocci10.dll`, and `oraociei10.dll` to the same folder where your project executable exists.
* Done!

(** You may be required to install Oracle Client in case of using `TNS Name`, 
      or other advanced functionalities that this library does not support.)

Note
----

* `EpOraLibrary` is tested on `OracleDB 10g` (`Windows XP 32bit`, and `Windows7 32bit`) only.
* In order to use with other versions of OracleDB, you may need to get compatible version of `OracleDB Instant Client SDK` and re-compile the `EpOraLibrary` with new headers and libs in it. (EpOraLibrary may not be compiled.)
* The dlls, required, may vary among the versions of `OracleDB Instant Client SDK`.

DOCUMENTATION
-------------

For detailed documentation, please download the full document [EpOraLibrary Documentation.chm](https://s3.amazonaws.com/elasticbeanstalk-us-east-1-052504677345/Downloads/EpOraLibrary/EpOraLibrary+Documentation.chm).

If the documentation file is not visible after finishing download,
* Solution 1.
  1. Double-click the .chm file.
  2. In the Open File-Security Warning dialog box, click to clear the Always ask before opening this file check box.
  3. Click Open.
* Solution 2.
  1. Right-click the CHM file, and then click Properties.
  2. Click Unblock.
  3. Double-click the .chm file to open the file.


USAGE
-----

To find the usage examples, please see the [wiki page](https://github.com/juhgiyo/EpOraLibrary/wiki/EpOraLibrary-Usage-Guide)


SPECIAL THANKS TO

606u <606u@dir.bg>
<http://606u.dir.bg/>

EpOraLibrary is developed by referencing oraLib 0.0.3, which was developed by 606u <606u@dir.bg> 
<http://606u.dir.bg/>
606u had kindly approved referencing his/her oraLib freely without any restriction for EpOraLibrary development.


REFERENCE
---------

* [OraLib](http://www.codeproject.com/Articles/1925/OraLib-a-lightweight-C-wrapper-over-Oracle-s-OCI-l)
* [EpLibrary 2.0](https://github.com/juhgiyo/EpLibrary)

DONATION
---------
[![Gratipay][gratipay-image]][gratipay-url]

or donation by Pledgie  
<a href='https://pledgie.com/campaigns/27766'><img alt='Click here to lend your support to: EpOraLibrary and make a donation at pledgie.com !' src='https://pledgie.com/campaigns/27766.png?skin_name=chrome' border='0' ></a>

LICENSE
-------

[The MIT License](http://opensource.org/licenses/mit-license.php)

Copyright (c) 2012-2013 Woong Gyu La <[juhgiyo@gmail.com](mailto:juhgiyo@gmail.com)>

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
[gratipay-image]: https://img.shields.io/gratipay/juhgiyo.svg?style=flat
[gratipay-url]: https://gratipay.com/juhgiyo/
