SRC			:= src
ZIP_NAME	:= 1-xzdrav00-xfrlic00.zip
DOC			:= doc
DOX_CONFIG	:= config-file


all: 
	@cd $(SRC)/ && $(MAKE) --no-print-directory

pack:
	zip -r $(ZIP_NAME) $(SRC) Makefile README.txt $(DOC)

%:
	@cd $(SRC)/ && $(MAKE) --no-print-directory $@

doxygen:
	@mkdir -p $(DOC)
	cd $(DOC) && doxygen -s -g $(DOX_CONFIG)
	cd $(DOC) && doxygen $(DOX_CONFIG)


clean:
	find $(DOC)/* -not -name '$(DOX_CONFIG)' -delete
	@cd $(SRC)/ && $(MAKE) --no-print-directory $@
	rm -f $(ZIP_NAME)