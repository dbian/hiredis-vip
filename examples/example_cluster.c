#include <stdio.h>
#include <hircluster.h>
int main()
{
    char *key = "key-a";
    char *field = "field-1";
    char *key1 = "key1";
    char *value1 = "value-1";
    char *key2 = "key1";
    char *value2 = "value-1";
    redisClusterContext *cc;
    char *ips = "127.0.0.1:8164,127.0.0.1:8163,127.0.0.1:8162,127.0.0.1:8161,127.0.0.1:8160,127.0.0.1:8159,127.0.0.1:8158,127.0.0.1:8157,127.0.0.1:8156,127.0.0.1:8155,127.0.0.1:8154,127.0.0.1:8153,127.0.0.1:8152,127.0.0.1:8151,127.0.0.1:8150,127.0.0.1:8149,127.0.0.1:8148,127.0.0.1:8147,127.0.0.1:8146,127.0.0.1:8145,127.0.0.1:8144,127.0.0.1:8143,127.0.0.1:8142,127.0.0.1:8141,127.0.0.1:8140,127.0.0.1:8139,127.0.0.1:8138,127.0.0.1:8137,127.0.0.1:8136,127.0.0.1:8135,127.0.0.1:8134,127.0.0.1:8133,127.0.0.1:8132,127.0.0.1:8131,127.0.0.1:8130,127.0.0.1:8129,127.0.0.1:8128,127.0.0.1:8127,127.0.0.1:8126,127.0.0.1:8125,127.0.0.1:8124,127.0.0.1:8123,127.0.0.1:8122,127.0.0.1:8121,127.0.0.1:8120,127.0.0.1:8119,127.0.0.1:8118,127.0.0.1:8117,127.0.0.1:8116,127.0.0.1:8115,127.0.0.1:8114,127.0.0.1:8113,127.0.0.1:8112,127.0.0.1:8111,127.0.0.1:8110,127.0.0.1:8109,127.0.0.1:8108,127.0.0.1:8107,127.0.0.1:8106,127.0.0.1:8105,127.0.0.1:8104,127.0.0.1:8103,127.0.0.1:8102,127.0.0.1:8101";

    cc = redisClusterContextInit();
    redisClusterSetOptionAuthPassword(cc, "aaa");
    // redisClusterSetMaxRedirect(cc, 100);
    // redisClusterSetOptionRouteUseSlots(cc);
    redisClusterSetOptionAddNodes(cc, ips);
    redisClusterConnect2(cc);

    if (cc == NULL || cc->err)
    {
        printf("connect error : %s\n", cc == NULL ? "NULL" : cc->errstr);
        return -1;
    }

    char *cmd = "return redis.call('get',KEYS[1]);";

    redisReply *reply = redisClusterCommand(cc, "eval %s 1 %s", cmd, "bar1"); //eval \"\" 0
    if (reply == NULL)
    {
        printf("reply1 is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }
    printf("reply1->type:%d, reply->str:%s\n", reply->type, reply->str);

    reply = redisClusterCommand(cc, "lrange %s 0 -1", "1"); //eval \"\" 0
    if (reply == NULL)
    {
        printf("reply1 is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }

    printf("reply2->type:%d, reply->str:%s\n", reply->type, reply->str);

    freeReplyObject(reply);
    char *cmd1 = "redis.call('del',KEYS[1]);for x=2,#ARGV do redis.call('rpush', KEYS[1],ARGV[x]);end;redis.call('expire',KEYS[1],ARGV[1]);return 1;";

    reply = redisClusterCommand(cc, "eval %s %d %s %d %b %s %s", cmd1, 1, "bar115", 400, "a21", 3, "aaa", "g2ggg");
    if (reply == NULL)
    {
        printf("reply2 is null[%s]\n", cc->errstr);
        redisClusterFree(cc);
        return -1;
    }

    printf("reply3->type:%d, reply->str:%s\n", reply->type, reply->str);

    int sz = 5;
    char **cmdlist = calloc(sz, sizeof(char *));
    size_t *cmdlen = calloc(sz, sizeof(size_t));
    cmdlist[0] = "eval";
    cmdlen[0] = 4;
    cmdlist[1] = cmd1;
    cmdlen[1] = strlen(cmdlist[1]);
    cmdlist[2] = "1";
    cmdlen[2] = strlen(cmdlist[2]);
    cmdlist[3] = "400";
    cmdlen[3] = strlen(cmdlist[3]);
    cmdlist[4] = "1255";
    cmdlen[4] = strlen(cmdlist[4]);
    // cmdlist[5] = "a21";
    // cmdlen[5] = strlen(cmdlist[5]);
    // cmdlist[6] = "a211";
    // cmdlen[6] = strlen(cmdlist[6]);
    reply = redisClusterCommandArgv(cc, sz, cmdlist, cmdlen);
    printf("reply4->type:%d, reply->str:%s\n", reply->type, reply->str);

    freeReplyObject(reply);

    redisClusterContext *cc2;

    cc2 = redisClusterContextInit();
    redisClusterSetOptionAuthPassword(cc2, "aaa");
    // redisClusterSetMaxRedirect(cc, 100);
    // redisClusterSetOptionRouteUseSlots(cc);
    redisClusterSetOptionAddNodes(cc2, ips);
    redisClusterConnect2(cc2);
    if (cc2 == NULL || cc2->err)
    {
        printf("connect error : %s\n", cc2 == NULL ? "NULL" : cc2->errstr);
        return -1;
    }

    cmd = "return redis.call('get',KEYS[1]);";

    reply = redisClusterCommand(cc2, "eval %s 1 %s", cmd, "bar1"); //eval \"\" 0
    if (reply == NULL)
    {
        printf("reply1 is null[%s]\n", cc2->errstr);
        redisClusterFree(cc2);
        return -1;
    }
    printf("reply1->type:%d, reply->str:%s\n", reply->type, reply->str);

    redisClusterFree(cc);
    redisClusterFree(cc2);

    return 0;
}