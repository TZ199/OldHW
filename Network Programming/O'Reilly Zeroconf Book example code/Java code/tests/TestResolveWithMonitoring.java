import com.apple.dnssd.*;

class TestResolveWithMonitoring implements ResolveListener, QueryListener
	{
	private DNSSDService monitorQ = null;
	
	// Display error message on failure
	public void operationFailed(DNSSDService service, int errorCode)
		{
		System.out.println("Resolve failed " + errorCode);
		System.exit(-1);
		}

	public void queryAnswered(DNSSDService query, int flags, int ifIndex,
		String fullName, int rrtype, int rrclass, byte[] rdata, int ttl)
		{
		if ((flags & 2) != 0)
			{
			boolean blankPrinted = false;
			TXTRecord txtRecord = new TXTRecord(rdata);
			for (int i = 0; i < txtRecord.size(); i++)
				{
				String key = txtRecord.getKey(i);
				String value = txtRecord.getValueAsString(i);
				if (key.length() > 0)
					{
					if (!blankPrinted)
						{
						blankPrinted = true;
						System.out.println();
						}
					System.out.println("\t" + key + "=" + value);
					}
				}
			}
		}

	// Display information when service is resolved
	public void serviceResolved(DNSSDService resolver, int flags, int ifIndex,
		String fullName, String hostName, int port, TXTRecord txtRecord)
		{
		System.out.println("Service Resolved: " + hostName + ":" + port);
		System.out.println("Flags: " + flags +
			", ifIndex: " + ifIndex + ", FQDN: " + fullName);

		// Now that we've got a resolve result,
		// start monitoring the TXT record and stop the resolve call.
		try { monitorQ = DNSSD.queryRecord(0, ifIndex, fullName, 16, 1, this); }
		catch (Exception e) { e.printStackTrace(); System.exit(-1); }
		resolver.stop();
		try { Thread.sleep(1); }
		catch (Exception e) { e.printStackTrace(); System.exit(-1); }
		}

	public TestResolveWithMonitoring(String name, String domain)
		throws DNSSDException, InterruptedException
		{
		System.out.println("TestResolveWithMonitoring Starting");
		DNSSDService r = DNSSD.resolve(0, DNSSD.ALL_INTERFACES,
			name, "_example._tcp", domain, this);
		System.out.println("TestResolveWithMonitoring Running");
		Thread.sleep(30000);
		System.out.println("TestResolveWithMonitoring Stopping");
		if (monitorQ == null) r.stop();
		else monitorQ.stop();
		Thread.sleep(1);
		}

	public static void main(String[] args)
		{
		if (args.length != 2)
			{
			System.out.println("Usage: java TestResolveWithMonitoring name dom");
			System.exit(-1);
			}
		else
			{
			try
				{
				new TestResolveWithMonitoring(args[0], args[1]);
				}
			catch (Exception e)
				{
				e.printStackTrace();
				System.exit(-1);
				} 
			}
		}
	}
