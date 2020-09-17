from PIL import Image
import os
import sys

path = '.'
files = os.listdir(path)
print(files)
jsonOutput = ""

for name in files:	
	if "." in name:
		continue
	list = os.listdir(name)	
	maxWidth = 0
	maxHeight = -1

	jsonOutput += "{ \"id\":\""+name+"\", \"filepath\":\""+name+".png\", \"boxes\": [ "
	elements = len(list)

	for img in list:
		image = Image.open(name + "\\" + img)
		width, height = image.size
		maxWidth += width
		if height > maxHeight:
			maxHeight = height
			
	newImage = Image.new('RGBA', (maxWidth, maxHeight), color = (0, 0, 0, 0))

	currWidth = 0
	counter = 1

	for img in list:
		image = Image.open(name + "\\" + img)	
		width, height = image.size
		newImage.paste(image, (currWidth, maxHeight - height))	
		if counter == elements:
			jsonOutput += "[" + str(currWidth) + ", " + str(maxHeight - height) + ", " + str(width) + ", " + str(height) + "] "
		else:
			jsonOutput += "[" + str(currWidth) + ", " + str(maxHeight - height) + ", " + str(width) + ", " + str(height) + "], "
		currWidth += width
		counter += 1

	jsonOutput += " ] }\n"	
	
	newImage.save(name + ".png")

json_file = open("allFilms.json", "w")
json_file.write(jsonOutput)

