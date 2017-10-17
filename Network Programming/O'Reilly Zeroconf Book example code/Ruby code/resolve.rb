require 'dnssd'
name = ARGV.shift
#name = "Stuart Cheshire's PowerBook G4"
puts "Resolving: #{name}"
resolver = DNSSD.resolve(name, "_http._tcp", "local") do |resolve_reply|
  puts "Resolve result: #{resolve_reply.inspect}"
end
puts "Resolver started"
sleep 30
resolver.stop
puts "Resolver stopped"
