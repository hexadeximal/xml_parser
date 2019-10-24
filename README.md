# xml_parser

parse an xml file using xpath and store results in an array.
example using the following xml

<?xml version="1.0" encoding="UTF-8" ?>
<test_parent>
	<child id='1'>
		<cvalue>cvalue 1</cvalue>
	</child>
	<child id='2'>
		<cvalue>cvalue 2</cvalue>
	</child>
	<child id='3'>
		<cvalue>cvalue 3</cvalue>
	</child>
</test_parent>

to get all cvalues:
  cvalues is now stored in result
  eval_xpath_expr(xml_doc, "//child/cvalue", &result, &count);

