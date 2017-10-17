import com.apple.dnssd.*;

class TestBrowse implements BrowseListener
	{
	// Display error message on failure
	public void operationFailed(DNSSDService service, int errorCode)
		{
		System.out.println("Browse failed " + errorCode);
		System.exit(-1);
		}

	// Display services we discover
	public void serviceFound(DNSSDService browser, int flags, int ifIndex, 
						String name, String regType, String domain)
		{
		System.out.println("Add flags:" + flags + ", ifIndex:" + ifIndex + 
			", Name:" + name + ", Type:" + regType + ", Domain:" + domain);
		}

	// Print a line when services go away
	public void serviceLost(DNSSDService browser, int flags, int ifIndex,
						String name, String regType, String domain)
		{
		System.out.println("Rmv flags:" + flags + ", ifIndex:" + ifIndex + 
			", Name:" + name + ", Type:" + regType + ", Domain:" + domain);
		}

	public TestBrowse() throws DNSSDException, InterruptedException
		{
		System.out.println("TestBrowse Starting");
		DNSSDService b = DNSSD.browse("_example._tcp", this);
		System.out.println("TestBrowse Running");
		Thread.sleep(30000);
		System.out.println("TestBrowse Stopping");
		b.stop();
		}

	public static void main(String[] args)
		{
		try { new TestBrowse(); }
		catch (Exception e)
			{
			//System.err.println(e.getMessage());
			e.printStackTrace();
			System.exit(-1);
			} 
		}
	}
