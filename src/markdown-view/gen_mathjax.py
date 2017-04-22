#!/usr/bin/env python
# coding: utf-8

import sys, os

def walk(cur_dir, mathjax_root, f, path):
	if os.path.isdir(path):
		entries = os.listdir(path)
		for e in entries:
			if e == '.' or e == '..':
				continue
			walk(cur_dir, mathjax_root, f, os.path.join(path, e))
	else:
		f.write('        <file alias="')
		f.write(os.path.relpath(path, mathjax_root).replace('\\', '/'))
		f.write('">')
		f.write(os.path.relpath(path, cur_dir).replace('\\', '/'))
		f.write('</file>\n')

def gen():
	cur_dir = os.path.split(sys.argv[0])[0]
	mathjax_root = os.path.abspath(os.path.join(cur_dir, '../../3rdparty/mathjax'))
	with open('mathjax.qrc', 'w') as f:
		f.write('<RCC>\n')
		f.write('    <qresource prefix="/mathjax">\n')
		walk(cur_dir, mathjax_root, f, mathjax_root)
		f.write('    </qresource>\n')
		f.write('</RCC>\n')

if __name__ == '__main__':
	gen()
