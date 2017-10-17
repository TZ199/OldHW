import java.net.*;
import com.apple.dnssd.*;

class TestRegisterWithUpdates implements RegisterListener
	{
	// Display error message on failure
	public void operationFailed(DNSSDService service, int errorCode)
		{
		System.out.println("Registration failed " + errorCode);
		}

	// Display registered name on success
	public void serviceRegistered(DNSSDRegistration registration, int flags,
		String serviceName, String regType, String domain)
		{
		System.out.println("Registered Name  : " + serviceName);
		System.out.println("           Type  : " + regType);
		System.out.println("           Domain: " + domain);
		}

	// Do the registration
	public TestRegisterWithUpdates(String name, int port)
		throws DNSSDException, InterruptedException
		{
		System.out.println("Registration Starting");
		System.out.println("Requested Name: " + name);
		System.out.println("          Port: " + port);

		DNSSDRegistration r = DNSSD.register(name, "_example._tcp", port, this);

		// New code to update TXT record begins here

		TXTRecord txtRecord = new TXTRecord();
		txtRecord.set("txtvers", "1");

		Thread.sleep(10000); // Wait ten seconds before updating TXT record
		txtRecord.set("status", "Ready");
		System.out.println("Ready");
		r.getTXTRecord().update(0, txtRecord.getRawBytes(), 0);
		
		Thread.sleep(5000);
		txtRecord.set("status", "Steady");
		System.out.println("Steady");
		r.getTXTRecord().update(0, txtRecord.getRawBytes(), 0);

		Thread.sleep(5000);
		txtRecord.set("status", "Go");
		System.out.println("Go");
		r.getTXTRecord().update(0, txtRecord.getRawBytes(), 0);

		// New code to update TXT record ends

		Thread.sleep(30000); // Wait thirty seconds, then exit
		System.out.println("Registration Stopping");
		r.stop();
		}

	public static void main(String[] args)
		{
		if (args.length > 1)
			{
			System.out.println("Usage: java TestRegisterWithUpdates name");
			System.exit(-1);
			}
		else
			{
			try
				{
				// If name specified, use it, else use default name
				String name = (args.length > 0) ? args[0] : null;
				// Let system allocate us an available port to listen on
				ServerSocket s = new ServerSocket(0);
				new TestRegisterWithUpdates(name, s.getLocalPort());
				}
			catch (Exception e)
				{
				e.printStackTrace();
				System.exit(-1);
				} 
			}
		}
	}
