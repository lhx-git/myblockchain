# myblockchain
a distributed system about blockchain 

1.blockserver使用coordinator worker模式，coordinator负责创建任务，并分配给worker线程处理，同时需要同步worker线程
2.transactionmanager 负责收集交易，将交易打包，通过回调函数，将交易加到coordinator的transactionbuffer中
3.reserve table，每组epoch交易，先构造一个保留表，然后根据保留表分析交易的waw、raw、war依赖（//todo，reservetable的销毁，避免内ucn泄漏）
4.database设计 使用单例模式，全局只有一个数据库对象。VersionedDB数据库接口中声明了commitupdates方法，用于提交交易。 
  为了实现数据库可插拔，抽象出一个DBStorage接口，每个数据库类都要实现DBStorage接口，sql语句 或者 kv数据库的操作全部实现在DBStorage里。
  数据库类继承了VersionedDB接口与DBStorageType，使用宏定义DBStorageType，可以控制数据库继承的DBStorage类型，从而实现数据库可插拔。
  为了支持复杂的关系数据库，需要实现ORM模块（简单实现）。
  每个插入操作都对应一个ORMInsert对象（用于生成写集）， 每个读操作都对应一个ORMQuery对象（会读数据库中的数据，并记录，用于生成读集）。
  实现了一个CCORMHelper类，用于构造一笔交易的读写集，并将该交易的读写集发送给 database对象，用于更新数据库。一笔交易对应一个helper对象。
5.transaction_executor,以组合的方式作为worker类的数据成员. 主要涉及两个方法:exectueList, commitList.
  transaction_executor有一个reserveTable对象, 每个epoch都有一个唯一的reserveTable对象, 所有的worker构造该保留表,并使用保留表决定
  废弃哪些事务.
6.comm模块:该模块共有2个角色,user_collector和comm_controller. user_collector负责收集客户端发送的交易,并将交易发送给epoch服务器,
  等待epoch服务器响应,然后将响应数据发送给comm_controller. comm_controller负责验证user_collector发送来的相应数据的正确性,并广播
  给其他节点, 同时收集其他节点发送的数据. comm模块最为TransactionImpl类的数据成员, TransactionImpl调用Comm模块的getTransaction方法, 
  获取有序的交易.





