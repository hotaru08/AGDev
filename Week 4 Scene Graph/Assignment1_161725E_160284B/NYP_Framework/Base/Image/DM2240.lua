-- Function to save
function SaveToLuaFile(outputString, overwrite, _type)
   print("Saving to different lua files ...")
   local f;						-- The file
   
	if _type == 1 then 			-- Highscore 
		if overwrite == 1 then		-- Wipe the contents with new data
			f = assert(io.open("Image/DM2240_HighScore.lua", "w"))
		elseif overwrite == 0 then	-- Append with new data
			f = assert(io.open("Image/DM2240_HighScore.lua", "a"))
		end
	elseif _type == 2 then 		-- Resolution
		if overwrite == 1 then		-- Wipe the contents with new data
			f = assert(io.open("Image/DM2240_Resolution.lua", "w"))
		elseif overwrite == 0 then	-- Append with new data
			f = assert(io.open("Image/DM2240_Resolution.lua", "a"))
		end
	elseif _type == 3 then 		-- Player Info
		if overwrite == 1 then		-- Wipe the contents with new data
			f = assert(io.open("Image/DM2240_Player.lua", "w"))
		elseif overwrite == 0 then	-- Append with new data
			f = assert(io.open("Image/DM2240_Player.lua", "a"))
		end
	elseif _type == 4 then 		-- Enemy
		if overwrite == 1 then		-- Wipe the contents with new data
			f = assert(io.open("Image/DM2240_Enemy.lua", "w"))
		elseif overwrite == 0 then	-- Append with new data
			f = assert(io.open("Image/DM2240_Enemy.lua", "a"))
		end
	elseif _type == 5 then 		-- Objects
		if overwrite == 1 then		-- Wipe the contents with new data
			f = assert(io.open("Image/DM2240_Objects.lua", "w"))
		elseif overwrite == 0 then	-- Append with new data
			f = assert(io.open("Image/DM2240_Objects.lua", "a"))
		end
						
	end -- end of checking for type
   
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   
   print("OK")
end

function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
	local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1)
	print(distanceSquare)
	return distanceSquare
end

function GetMinMax(...)
	local minValue = -1
	local maxValue = -1
	local avgValue = -1
	local numValues = 0
	for i,v in ipairs(arg) do
		if minValue == -1 then
			minValue = v
		elseif v < minValue then
			minValue = v
		end

		if maxValue == -1 then
			maxValue = v
		elseif v > maxValue then
			maxValue = v
		end
		
		avgValue = avgValue + v
		numValues = numValues + 1
    end
	avgValue = avgValue / numValues
	print(minValue, maxValue, avgValue, numValues)
	return minValue, maxValue, avgValue, numValues
end
