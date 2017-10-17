import java.net.*;
import com.apple.dnssd.*;

class TestRegisterWithAttributes implements RegisterListener
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
	public TestRegisterWithAttributes(String name, int port)
		throws DNSSDException, InterruptedException
		{
		System.out.println("Registration Starting");
		System.out.println("Requested Name: " + name);
		System.out.println("          Port: " + port);

		// New code to register with TXT record begins here
		TXTRecord txtRecord = new TXTRecord();
		txtRecord.set("txtvers", "1");
		txtRecord.set("status", "ready");
		txtRecord.set("difficulty", "medium");
		DNSSDRegistration r = DNSSD.register(0, DNSSD.ALL_INTERFACES,
			name, "_example._tcp", null,	// Name, type, and domain
			null, port,						// Target host and port
			txtRecord, this);				// TXT record and listener object
		// New code to register with TXT record ends

		Thread.sleep(30000); // Wait thirty seconds, then exit
		System.out.println("Registration Stopping");
		r.stop();
		}

	public static void main(String[] args)
		{
		if (args.length > 1)
			{
			System.out.println("Usage: java TestRegisterWithAttributes name");
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
				new TestRegisterWithAttributes(name, s.getLocalPort());
				}
			catch (Exception e)
				{
				e.printStackTrace();
				System.exit(-1);
				} 
			}
		}
	}
