#!/usr/bin/ruby

columns = {}
names = ARGV
unless names.include?('time')
  names << 'time'
end
names.each { |name| columns[name] = 'x' }
columns['time'] = nil

$stdout.puts names.join(", ")

time = nil
$stdin.each_line do |line|
  next if line =~ /^\/\//  # discard comments.
  name, value = line.strip.split(', ')
  next unless names.include?(name)

  if 'time' == name
    time = value
  else
    if time != columns['time']
      if columns['time']
        $stdout.puts names.collect { |n| columns[n] }.join(', ')
      end
      columns.each { |n,v| columns[n] = 'x' }
      columns['time'] = time
    end
    columns[name] = value
  end
end

# Emit last line.
if columns['time']
  $stdout.puts names.collect { |n| columns[n] }.join(', ')
end
