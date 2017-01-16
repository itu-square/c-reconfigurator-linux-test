require 'fileutils'

puts "Import"

if (ARGV[0] == nil || ARGV[1] == nil)
	puts "Nothing to import."
	exit
end

file = ARGV[0]
linux = "/home/alex/reconfigurator/linux-4.9/"

puts "Importing [#{file}] from [#{linux}]."

File.open(file + ".inc", "r").each_line do |line|
	if (line.length>1 && (line =~ /\.* /)==0 && (line =~ /\.* \/usr/)==nil)

		dir = (Dir.pwd + "/" + line.sub(/\.* /, ""))[0..(Dir.pwd + "/" + line.sub(/\.* /, "")).rindex(/\//)].sub(/\.\//, "")
		filein = (linux + line.sub(/\.* /, "").sub("\n", "")).sub(/\.\//, "")
		fileout = (Dir.pwd + "/" + line.sub(/\.* /, "").sub("\n", "")).sub(/\.\//, "")

		puts
		puts "line : [#{line}]"
		puts "mkdir: [#{dir}]"
		puts "read : [#{filein}]"
		puts "write: [#{fileout}]"
		
		puts "read (E): " + File.exists?(filein).to_s
		puts "write(E): " + File.exists?(fileout).to_s
		
		FileUtils.mkdir_p dir

		File.open(fileout, 'w') do |out|
		File.open(filein, "r").each_line do |inln|
			out.write("// /*AFLA*/ " + inln)
		end
		end
	end
end

dir = (Dir.pwd + "/" + file)[0..(Dir.pwd + "/" + file).rindex(/\//)].sub(/\.\//, "")
filein = (linux + file)
fileout = (Dir.pwd + "/" + file)

puts
puts "file : [#{file}]"
puts "mkdir: [#{dir}]"
puts "read : [#{filein}]"
puts "write: [#{fileout}]"
		
puts "read (E): " + File.exists?(filein).to_s
puts "write(E): " + File.exists?(fileout).to_s
		
FileUtils.mkdir_p dir

File.open(fileout, 'w') do |out|
File.open(filein, "r").each_line do |inln|
	out.write("// /*AFLA*/ " + inln)
end
end

puts "Done"