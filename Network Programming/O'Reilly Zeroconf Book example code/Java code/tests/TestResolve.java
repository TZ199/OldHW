import com.apple.dnssd.*;

class TestResolve implements ResolveListener
	{
	// Display error message on failure
	public void operationFailed(DNSSDService service, int errorCode)
		{
		System.out.println("Resolve failed " + errorCode);
		System.exit(-1);
		}

	// Display information when service is resolved
	public void serviceResolved(DNSSDService resolver, int flags, int ifIndex,
		String fullName, String hostName, int port, TXTRecord txtRecord)
		{
		System.out.println("Service Resolved: " + hostName + ":" + port);
		System.out.println("Flags: " + flags +
			", ifIndex: " + ifIndex + ", FQDN: " + fullName);

		for (int i = 0; i < txtRecord.size(); i++)
			{
			String key = txtRecord.getKey(i);
			String value = txtRecord.getValueAsString(i);
			if (key.length() > 0) System.out.println("\t" + key + "=" + value);
			}
		}

	public TestResolve(String name, String domain)
		throws DNSSDException, InterruptedException
		{
		System.out.println("TestResolve Starting");
		DNSSDService r = DNSSD.resolve(0, DNSSD.ALL_INTERFACES,
			name, "_example._tcp", domain, this);
		System.out.println("TestResolve Running");
		Thread.sleep(5000);
		System.out.println("TestResolve Stopping");
		r.stop();
		}

	public static void main(String[] args)
		{
		if (args.length != 2)
			{
			System.out.println("Usage: java TestResolve name domain");
			System.exit(-1);
			}
		else
			{
			try
				{
				new TestResolve(args[0], args[1]);
				}
			catch (Exception e)
				{
				e.printStackTrace();
				System.exit(-1);
				} 
			}
		}
	}
