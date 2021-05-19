SRC			:= src
ZIP_NAME	:= 1-xzdrav00-xfrlic00.zip
DOC			:= doc
DOX_CONFIG	:= Doxyfile
ZIP_DIRS	:= $(SRC) examples
ZIP_FILES	:= Makefile README.md $(DOX_CONFIG) $(DOC)

all: 
	@cd $(SRC)/application/ && $(MAKE) --no-print-directory 
	@cd $(SRC)/simulator/ && $(MAKE) --no-print-directory

pack:
# 	@rm -f $(ZIP_NAME)
	zip -r $(ZIP_NAME) $(ZIP_DIRS)
	zip $(ZIP_NAME) $(ZIP_FILES) 

%:
	@cd $(SRC)/application/ && $(MAKE) --no-print-directory $@

simulator:
	@cd $(SRC)/simulator/ && $(MAKE) --no-print-directory


doxygen:
	@mkdir -p $(DOC)
	doxygen $(DOX_CONFIG)


clean:
	rm -rf $(DOC)/*
	@cd $(SRC)/application/ && $(MAKE) --no-print-directory $@
	rm -f $(ZIP_NAME) 