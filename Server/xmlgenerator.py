import xml.etree.ElementTree as ET

class xmlgenerator:
	def __init__(self):
		self.xmltree = ET.Element('articles')
		
	def add(self, **params):
		sub = ET.SubElement(self.xmltree,'article')
		sub_author = ET.SubElement(sub, 'author')
		sub_author.text = params['author']
		sub_date = ET.SubElement(sub, 'date')
		sub_date.text = params['date']
		sub_title = ET.SubElement(sub, 'title')
		sub_title.text = params['title']
		sub_content = ET.SubElement(sub, 'content')
		sub_content.text = params['content']

	def out(self):
		ET.dump(self.xmltree)
	