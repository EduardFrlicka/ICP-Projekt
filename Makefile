SRC			:= src
ZIP_NAME	:= 1-xzdrav00-xfrlic00.zip
DOC			:= doc
DOX_CONFIG	:= config-file


all: 
	@cd $(SRC)/application/ && $(MAKE) --no-print-directory 
	@cd $(SRC)/simulator/ && $(MAKE) --no-print-directory

pack:
	zip -r $(ZIP_NAME) $(SRC) Makefile README.txt $(DOC)

%:
	@cd $(SRC)/application/ && $(MAKE) --no-print-directory $@

simulator:
	@cd $(SRC)/simulator/ && $(MAKE) --no-print-directory


doxygen:
	@mkdir -p $(DOC)
	cd $(DOC) && doxygen -s -g $(DOX_CONFIG)
	cd $(DOC) && doxygen $(DOX_CONFIG)


clean:
	find $(DOC)/* -not -name '$(DOX_CONFIG)' -delete
	@cd $(SRC)/application/ && $(MAKE) --no-print-directory $@
	rm -f $(ZIP_NAME)