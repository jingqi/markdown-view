
TEMPLATE = subdirs

SUBDIRS += \
    markdown-viewer \
    test-markdown-viewer

test-markdown-viewer.depends = markdown-viewer
