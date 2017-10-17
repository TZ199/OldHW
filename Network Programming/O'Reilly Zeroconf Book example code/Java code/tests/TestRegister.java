import java.net.*;
import com.apple.dnssd.*;

class TestRegister implements RegisterListener
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
	public TestRegister(String name, int port)
		throws DNSSDException, InterruptedException
		{
		System.out.println("Registration Starting");
		System.out.println("Requested Name: " + name);
		System.out.println("          Port: " + port);
		//DNSSDRegistration r = DNSSD.register("Moët & Chandon", "_example._tcp", 9099, this);
		DNSSDRegistration r = DNSSD.register(name, "_example._tcp", port, this);
		Thread.sleep(3000);
		DNSRecord rec = r.getTXTRecord();
		byte[] x = new byte[1];
		DNSRecord rec2 = r.addRecord(0, 0, x, 0);
		Thread.sleep(30000); // Wait thirty seconds, then exit
		System.out.println("Registration Stopping");
		r.stop();
		}

	public static void main(String[] args)
		{
		if (args.length > 1)
			{
			System.out.println("Usage: java TestRegister name");
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
				new TestRegister(name, s.getLocalPort());
				}
			catch (Exception e)
				{
				e.printStackTrace();
				System.exit(-1);
				} 
			}
		}
	}
