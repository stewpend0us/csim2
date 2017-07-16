clean:
	if [ -d "$(OBJ_DIR)" ]; then \
		rm -r "$(OBJ_DIR)"; \
	fi

dist-clean: clean
	rm -f $(TARGET)

-include $(DEPENDS)