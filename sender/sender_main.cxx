#include <stdio.h>
#include <memutils/message/Message.h>
#include "../include/msgq_id.h"
#include "../include/msgq_pool.h"

using namespace std;

struct message_t
{
  int num;
  char str[20];
};

extern "C" int main(int argc, FAR char *argv[])
{

  // 初期化
  err_t err = MsgLib::initFirst(NUM_MSGQ_POOLS, MSGQ_TOP_DRM);
  if (err != ERR_OK && err != ERR_STS)
  {
    printf("MsgLib::initFirst error: %x\n", err);
    return 1; // 初期化エラーの場合、終了する
  }

  while (ERR_OK != MsgLib::initPerCpu())
  {
    printf("MsgLib::initPerCpu error: retrying...\n");
    usleep(1000 * 1000); // 一定時間待ってから再試行
  }
  printf("sender_init:OK\n");
  while (ERR_OK != MsgLib::initPerCpu())
    ;
  printf("Size of struct message_t: %zu bytes\n", sizeof(struct message_t));
  int cnt = 0;
  while (1)
  {
    MsgQueId send_id = MSGQ_RESPONSE; // Assign ID that be sent to a variable "send_id".
    MsgQueId ret_id = MSGQ_REQUEST;   // Assign ID that will return to a variable "self_id".
    message_t message = {cnt, "Hello, World!"};
    MSG_TYPE msg_type = MSG_TYPE_RESPONSE;
    err_t err = MsgLib::send<message_t>(send_id, MsgPriNormal, msg_type, ret_id, message);
    if (err != ERR_OK)
    {
      printf("send error: %x\n", err);
    }
    printf("send_msg: %d %s\n", message.num, message.str);
    usleep(1000 * 1000);
    cnt++;
    // printf("ERR_DATA_VAL: %x\n", ERR_DATA_VAL);
    // printf("ERR_EVT_BAD; %x\n", ERR_EVT_BAD);
  }
  return 0;
}
