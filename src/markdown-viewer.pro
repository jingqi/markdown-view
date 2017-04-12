
TEMPLATE = subdirs

SUBDIRS += \
    discount \
    markdown-viewer \
    test-markdown-viewer

markdown-viewer.depends = discount
test-markdown-viewer.depends = markdown-viewer
