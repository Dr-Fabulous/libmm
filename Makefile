MK:=

define DEC_MK_RULE
$(1): $$(foreach DEP,$(2),$$(DEP))
	$$(MAKE) -C $(1) all

$(1)-clean:
	$$(MAKE) -C $(1) clean

.PHONY: $(1) $(1)-clean
MK+=$(1)
endef

$(eval $(call DEC_MK_RULE,libmm,))
$(eval $(call DEC_MK_RULE,unit-test,libmm))

all: libmm unit-test

clean: $(foreach DEP,$(MK),$(DEP)-clean)

test: unit-test
	unit-test/unit-test

.PHONY: all clean test
