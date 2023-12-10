0x1ac19a0
0x1ac7b20

0x1ac7b20
cq_init_next 0x1ac7c70


0x1acf730
0x1acfee8

struct ServerCompletionQueue : public CompletionQueue
{

};

struct CompletionQueue 
{
      grpc_completion_queue* cq_;  // owned

};

struct grpc_completion_queue
{

};
struct cq_next_data
{
  CqEventQueue queue;
};
class CqEventQueue {
  MultiProducerSingleConsumerQueue queue_;
};

struct MultiProducerSingleConsumerQueue
{
    /* data */
};

// 在内存分布上

grpc_completion_queue *a;

cq_next_data *b = (cq_next_data*) (a+1);




1.ServerBuilder 设置监听地址, 注册服务(service),  创建ServerCompletionQueue(cq)
2.ServerBuilder::BuildAndStart 来创建Server
  将上面的cq注册到Server中, 在Server中使用cqs_保存
  将上面的service注册到Server, RegisterService
3.RegisterServices的工作
  将Service中的Method, 也就是接口都加工成Server::RegisteredMethod, 放入server中的registered_methods_,
  Server::RegisteredMethod的对象也会被Service的Method所引用, 保存在method的RpcServiceMethod::server_tag_中
  将service的那么存入Server的services_中

4. 通过Service注册接口回调, 通过RequestAsyncUnary(RequestAsyncCall)注册
  注册回调时, 要加上 cq(已经保存在Server的cqs了), request和response的指针, context, 以及一个tag(userdata)
  当有事件出现时, 通过cq可以取到tag,  然后调用tag的方法
  
  RequestAsyncCall的工作
  PayloadAsyncRequest : public RegisteredAsyncRequest : public BaseAsyncRequest
  将service引出的tag绑定到PayloadAsyncRequest对象上, 以PayloadAsyncRequest对象为载体, 将PayloadAsyncRequest保存到RequestedCall对象中,
  RequestedCall以第一个成员mpscq_node插入到 接口对应的RegisteredMethod中的匹配队列中







通过service调用RequestAsyncCall, 带上加入到server中的completequeue,






