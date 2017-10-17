require 'dnssd'
browser = DNSSD.browse('_http._tcp') do |browse_reply|
  if (browse_reply.flags.to_i & DNSSD::Flags::Add) != 0
    puts "Add: #{browse_reply.inspect}"
  else
    puts "Rmv: #{browse_reply.inspect}"
  end
end
puts "Browsing started"
sleep 30
browser.stop
puts "Browsing stopped"
