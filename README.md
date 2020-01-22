<p align="left">
  <a href="https://tarantool.io/en/">
    <img
      alt="Tarantool"
      src="https://i.imgur.com/BUYqImx.png"
      width="400"
    />
  </a>
</p>

**A feature-rich in-memory database.**  It has the performance of a key-value
cache with the functionality you would expect from a regular database:

* SQL
* ACID
* primary and secondary indexes
* disk engine
* stored procedures
* replication
* sharding (as an external module)
* and a wealth of modules and connectors from popular languages

Another Tarantool feature is the ability to write code in Lua, a general purpose
programming language, right inside the database. You can write custom expiration
strategies and any business logic, and synchronize data with other data sources.

You can use Tarantool to replace Redis when
you need secondary indexes, a different structure of storage data, SQL, etc.

For more details, please see the
[Tarantool documentation](https://www.tarantool.io/en/doc/2.2/).

## Getting started

### Get hold of Tarantool

* Try online:

  Visit our playground at https://www.tarantool.io/en/try-dev/

* Use a Docker image:

  ```bash
  docker run -d -p 3301:3301 -v $(pwd):/var/lib/tarantool tarantool/tarantool:2
  ```

* Install locally:

  * On Ubuntu/Fedora/Debian/Centos:

    ```bash
    curl -L http://tarantool.io/installer.sh | VER=2.2 sudo bash
    ```

  * On Mac OS using [Homebrew](https://brew.sh):

    ```bash
    brew install tarantool
    ```

  For more details, please visit https://tarantool.io/download.

* Build from source:

  Please see
  [detailed instructions](https://tarantool.io/doc/2.2/dev_guide/building_from_source/)
  in the Tarantool documentation.

### Connectors

We have connectors for many popular languages:

* [Python](https://github.com/igorcoding/asynctnt)
* [PHP](https://github.com/tarantool-php/client)
* [Java](http://github.com/tarantool/tarantool-java/)
* [Go](https://github.com/tarantool/go-tarantool)

[Check out connector details for other languages](https://www.tarantool.io/doc/2.2/book/connectors/)
and connect to your current instance of Tarantool.

## Use cases

* Transaction cache, OLTP
* Realtime analytics
* ETL
* Profile/session storage
* Queue
* IoT

## Ready-made examples

We have a frawework for creating of distributed apps called [Tarantool Cartridge](https://github.com/tarantool/cartridge).
See the [tarantool/examples](https://github.com/tarantool/examples) repository
with source code of typical projects based on this framework.

## Who uses Tarantool

* Nokia uses Tarantool as part of their IoT platform
  (in-memory database + Lua scripting)
* Russian Enterprise: banks, telecoms, retails
* Mail.Ru, a major Russian Internet company, uses Tarantool for its email
  service, ad systems, social services, media projects
* Mastercard
* Avito, Russian largest classified advertisements website
* and many other companies

## Community

Join our Telegram [news channel](https://t.me/tarantool_news) and chats:

* in [Russian](https://teleg.run/tarantoolru)
* in [English](https://teleg.run/tarantool)

Report bugs at https://github.com/tarantool/tarantool/issues

Submit your patches as instructed
[here](https://www.tarantool.io/en/doc/2.2/dev_guide/developer_guidelines/#how-to-submit-a-patch-for-review).

Ask your questions in tarantool@googlegroups.com.

Discuss development issues (architecture, features, and other implementation details) in https://lists.tarantool.org/mailman/listinfo/tarantool-discussions/.
