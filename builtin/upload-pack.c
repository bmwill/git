#include "cache.h"
#include "builtin.h"
#include "exec_cmd.h"
#include "pkt-line.h"
#include "parse-options.h"
#include "protocol.h"
#include "upload-pack.h"

static const char * const upload_pack_usage[] = {
	N_("git upload-pack [<options>] <dir>"),
	NULL
};

int cmd_upload_pack(int argc, const char **argv, const char *prefix)
{
	const char *dir;
	int strict = 0;
	struct upload_pack_options opts = { 0 };
	struct option options[] = {
		OPT_BOOL(0, "stateless-rpc", &opts.stateless_rpc,
			 N_("quit after a single request/response exchange")),
		OPT_BOOL(0, "advertise-refs", &opts.advertise_refs,
			 N_("exit immediately after initial ref advertisement")),
		OPT_BOOL(0, "strict", &strict,
			 N_("do not try <directory>/.git/ if <directory> is no Git directory")),
		OPT_INTEGER(0, "timeout", &opts.timeout,
			    N_("interrupt transfer after <n> seconds of inactivity")),
		OPT_END()
	};

	packet_trace_identity("upload-pack");
	check_replace_refs = 0;

	argc = parse_options(argc, argv, NULL, options, upload_pack_usage, 0);

	if (argc != 1)
		usage_with_options(upload_pack_usage, options);

	if (opts.timeout)
		opts.daemon_mode = 1;

	setup_path();

	dir = argv[0];

	if (!enter_repo(dir, strict))
		die("'%s' does not appear to be a git repository", dir);

	switch (determine_protocol_version_server()) {
	case protocol_v2:
		/*
		 * fetch support for protocol v2 has not been implemented yet,
		 * so ignore the request to use v2 and fallback to using v0.
		 */
		upload_pack(&opts);
		break;
	case protocol_v1:
		/*
		 * v1 is just the original protocol with a version string,
		 * so just fall through after writing the version string.
		 */
		if (opts.advertise_refs || !opts.stateless_rpc)
			packet_write_fmt(1, "version 1\n");

		/* fallthrough */
	case protocol_v0:
		upload_pack(&opts);
		break;
	case protocol_unknown_version:
		BUG("unknown protocol version");
	}

	return 0;
}
