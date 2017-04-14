
TEMPLATE = subdirs

SUBDIRS += \
    discount \
    hoedown \
    markdown-view \
    test-markdown-view

markdown-view.depends = discount hoedown
test-markdown-view.depends = markdown-view
