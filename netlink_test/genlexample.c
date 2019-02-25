#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/genetlink.h>
#include <net/genetlink.h>
#include <net/sock.h>

#include "genlexample.h"

static int myvar;

static struct genl_family genl_example_family = {
	.module = THIS_MODULE,
	.ops = NULL,
	.mcgrps = NULL,
	.n_ops = 0,
	.n_mcgrps = 0,
	.name = "nlex",
	.hdrsize = 0,
	.version = 1,
	.maxattr = NLE_MAX,
};

static int
genl_get_myvar(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *msg;
	void *hdr;

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (msg == NULL)
		return -ENOMEM;

	hdr = genlmsg_put(msg, info->snd_portid, info->snd_seq,
			  &genl_example_family, 0, NLEX_CMD_UPD);
	if (hdr == NULL)
		goto nlmsg_failure;

	if (nla_put_u32(msg, NLE_MYVAR, myvar))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_unicast(sock_net(skb->sk), msg, info->snd_portid);

	return 0;

nlmsg_failure:
nla_put_failure:
	genlmsg_cancel(msg, hdr);
	kfree_skb(msg);
	return -ENOBUFS;
}

static int
genl_upd_myvar(struct sk_buff *skb, struct genl_info *info)
{
	struct sk_buff *msg;
	void *hdr;

	if (!info->attrs[NLE_MYVAR])
		return -EINVAL;

	myvar = nla_get_u32(info->attrs[NLE_MYVAR]);

	msg = nlmsg_new(NLMSG_DEFAULT_SIZE, GFP_KERNEL);
	if (msg == NULL)
		return -ENOMEM;

	hdr = genlmsg_put(msg, 0, 0, &genl_example_family, 0, NLEX_CMD_UPD);
	if (hdr == NULL)
		goto nlmsg_failure;

	if (nla_put_u32(msg, NLE_MYVAR, myvar))
		goto nla_put_failure;

	genlmsg_end(msg, hdr);

	genlmsg_multicast(&genl_example_family, msg, 0, NLEX_GRP_MYVAR, GFP_KERNEL);  // FIXME: The genlmsg_multicast() API has been changed by kernel developers. I'm not sure where to get a group number.
	return 0;

nlmsg_failure:
nla_put_failure:
	genlmsg_cancel(msg, hdr);
	kfree_skb(msg);
	return -ENOBUFS;
}

static struct genl_ops genl_example_ops[] = {
	{
		.cmd = NLEX_CMD_GET,
		.doit = genl_get_myvar,
	},
	{
		.cmd = NLEX_CMD_UPD,
		.doit = genl_upd_myvar,
	},
};

static struct genl_multicast_group genl_example_mcgrps[1] = {
	{
		.name = "example",
	},
};

static int __init nlexample_init(void)
{
	genl_example_family.ops = genl_example_ops;
	genl_example_family.n_ops =
		sizeof(genl_example_ops) / sizeof(genl_example_ops[0]);
	genl_example_family.mcgrps = genl_example_mcgrps;
	genl_example_family.n_mcgrps =
		sizeof(genl_example_mcgrps) / sizeof(genl_example_mcgrps[0]);
	return genl_register_family(&genl_example_family);
}

void __exit nlexample_exit(void)
{
	genl_unregister_family(&genl_example_family);
}

module_init(nlexample_init);
module_exit(nlexample_exit);

MODULE_LICENSE("GPL");
