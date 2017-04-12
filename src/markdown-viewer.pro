
TEMPLATE = subdirs

SUBDIRS += \
    discount \
    hoedown \
    markdown-viewer \
    test-markdown-viewer

markdown-viewer.depends = discount hoedown
test-markdown-viewer.depends = markdown-viewer
