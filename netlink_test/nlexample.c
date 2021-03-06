#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/skbuff.h>
#include <linux/netlink.h>
#include <net/net_namespace.h>

#ifndef NETLINK_EXAMPLE
#define NETLINK_EXAMPLE 21
#endif

#define NLEX_GRP_MAX	0

static struct sock *nlsk;
static int myvar = -1;  // We should set an initial value for debugging...

static void
nl_callback(struct sk_buff *skb)
{
	pr_info("the current value is %d\n", myvar);
}

static int __init nlexample_init(void)
{
	struct netlink_kernel_cfg cfg = {
		.groups = NLEX_GRP_MAX,
		.input	= nl_callback,
	};

	pr_debug("init nlexample module\n");

	pr_debug("init nlexample module before create\n");
	nlsk = netlink_kernel_create(&init_net, NETLINK_EXAMPLE, &cfg);
	if (nlsk == NULL) {
		pr_err("Can't create netlink example\n");
		return -ENOMEM;
	}
	pr_err("nlsk ptr is %p\n", nlsk);
	return 0;
}

static void __exit nlexample_exit(void)
{
	netlink_kernel_release(nlsk);
	pr_debug("exit nlexample module\n");
}

module_init(nlexample_init);
module_exit(nlexample_exit);

MODULE_AUTHOR("Pablo Neira Ayuso <pablo@netfilter.org>");
MODULE_LICENSE("GPL");
