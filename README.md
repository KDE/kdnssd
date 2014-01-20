# KDE DNS-SD

Network service discovery using Zeroconf

## Introduction

KDNSSD is a library for handling the DNS-based Service Discovery Protocol
(DNS-SD), the layer of [Zeroconf](http://www.zeroconf.org) that allows network
services, such as printers, to be discovered without any user intervention or
centralized infrastructure.


## Usage

If you are using CMake, you need to have

    find_package(KF5DNSSD NO_MODULE)

(or similar) in your CMakeLists.txt file, and you need to link to KF5::DNSSD.

If you are writing an application that wants to discover services on the
network, use DNSSD::ServiceBrowser.  You can also find available service types
using ServiceTypeBrowser.

If you want to announce the availability of a service provided by your
application, use DNSSD::PublicService.

DNSSD::DomainBrowser allows you to find domains (other than the local one)
recommended for browsing or publishing to.

Note that DNSSD::ServiceBrowser::isAvailable() provides information about the
availability of the services provided by this library generally, not just for
browsing services.


## More information

More information about DNS-SD can be found in the [online
documentation][appledocs] for Apple's implementation of Zeroconf,
[Bonjour](http://www.apple.com/support/bonjour/).

[appledocs]: https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/NetServices/Articles/about.html


## Links

- Home page: <https://projects.kde.org/projects/frameworks/kdnssd-frameworks>
- Mailing list: <https://mail.kde.org/mailman/listinfo/kde-frameworks-devel>
- IRC channel: #kde-devel on Freenode
- Git repository: <https://projects.kde.org/projects/frameworks/kdnssd-framework/repository>
