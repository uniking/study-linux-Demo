local lfs = require"lfs"

function attrdir (path)
    for file in lfs.dir(path) do
        if file ~= "." and file ~= ".." then
            local f = path..'/'..file
            print (f)
            local attr = lfs.attributes (f)
            assert (type(attr) == "table")
            if attr.mode == "directory" then
                attrdir (f)
            else
                --for name, value in pairs(attr) do
                --    print (name, value)
                --end
					local v=""
					for _, value in pairs(attr) do						
						v = v .. value
						v = v .. " ";
					end
					print(v)
            end
        end
    end
end

function get_file_list(path)
	local list = {}
	local function cget(path)
		for file in lfs.dir(path) do
			if file ~= "." and file ~= ".." then
				local f = path..'/'..file
				local attr = lfs.attributes (f)
				assert (type(attr) == "table")
				if attr.mode == "directory" then
					cget(f)
				else
					table.insert(list, f)
				end
			end
		end
	end

	cget(path)
	return list
end

function get_file(path)
	local index = 0
	local list=get_file_list(path)
	return function()
			index = index+1
			return list[index]
			end
end

--attrdir (".")
for n in get_file(".") do
	print(n)
end
