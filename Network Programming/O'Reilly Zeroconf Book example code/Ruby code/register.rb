require 'dnssd'
registration = DNSSD.register("", "_http._tcp", nil, 8080) do |register_reply|
  puts "Registration result: #{register_reply.inspect}"
end
puts "Registration started"
sleep 30
registration.stop
puts "Registration stopped"
