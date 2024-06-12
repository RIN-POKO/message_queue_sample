#include <stdio.h>
#include <memutils/message/Message.h>
#include "../include/msgq_id.h"
#include "../include/msgq_pool.h"

uint8_t AutoGenMesgBuff[1204];
MsgQueDef MsgqPoolDefs[NUM_MSGQ_POOLS] =
    {
        /* n_drm, n_size, n_num, h_drm, h_size, h_num */

        {0x00000000, 0, 0, 0x00000000, 0, 0, 0},                    /* MSGQ_NULL */
        {(drm_t)AutoGenMesgBuff + 0xcc, 100, 5, 0xffffffff, 0, 0},  /* MSGQ_REQUEST */
        {(drm_t)AutoGenMesgBuff + 0x2c0, 100, 5, 0xffffffff, 0, 0}, /* MSGQ_RESPONSE */
};

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
  printf("receiver_init:OK\n");

  while (1)
  {
    MsgQueId id = MSGQ_RESPONSE; // Assign the created ID to a variable "id".
    MsgQueBlock *que;
    MsgPacket *msg;
    MSG_TYPE msg_type = MSG_TYPE_RESPONSE;
    err_t err = MsgLib::referMsgQueBlock(id, &que);
    err = que->recv(TIME_FOREVER, &msg);
    if (msg->getType() == msg_type)
    {                                                  // Check that the message type is as expected or not.
      message_t message = msg->moveParam<message_t>(); // get an instance of type Object from Message packet.
      printf("receive_msg: %d %s\n", message.num, message.str);
      err = que->pop(); // Release the message block.
      usleep(1000 * 1000);
    }
  }
  return 0;
}