LLVMGenericValueRef intArg(int n) {
    return LLVMCreateGenericValueOfInt(LLVMInt64Type(), n, false);
}

int runLLVMModule(Irgen *irgen) {

    // create an execution engine   
    LLVMExecutionEngineRef engine;
    char *error = (char *)NULL;

    # if 1
        // Initialize jit
        LLVMLinkInMCJIT();
    # else
        // Initialize intepreter
        LLVMLinkInInterpreter();
    # endif

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();
    LLVMInitializeNativeAsmParser();

    assert(LLVMCreateExecutionEngineForModule(&engine, irgen->module, &error) == 0);
    assert(error == NULL);

    LLVMValueRef mainFunc; 
    LLVMFindFunction(engine, "main", &mainFunc);

    int res = LLVMRunFunctionAsMain(engine, mainFunc, 0, NULL, NULL);

    LLVMDisposeExecutionEngine(engine);
    return res;
}

void TEST_ERROR(char *src, TokenType type) {
    parser *p = new_parser(Lex(src));
    ast_unit *f = parse_file(p);
    
    ASSERT_EQ(queue_size(p->error_queue), 1);
    parser_error *err = (parser_error *)queue_pop_back(p->error_queue);
    ASSERT_EQ(err->expect_token.type, type);
}

void TEST_MODULE(char *src, int out) {
    /* generate module */
    parser *p = new_parser(Lex(src));
    ast_unit *f = parse_file(p);
    
    Irgen *irgen = NewIrgen();
    
    for (int i = 0; i < f->dclCount; i++) {
        CompileFunction(irgen, f->dcls[i]);
    }

    // LLVMDumpModule(irgen->module);

    /* check for errors in module */
    char *error = (char *)NULL;
    LLVMVerifyModule(irgen->module, LLVMPrintMessageAction, &error);
    LLVMDisposeMessage(error);

    /* run the function */
    int res = runLLVMModule(irgen);
    ASSERT_EQ(out, res);

    /* dispose of builder */
    LLVMDisposeBuilder(irgen->builder);
}

char *loadTest(std::string name) {
    // build path to the file
    char *cname = (char *)name.c_str();
    char *pre = (char *)"../tests/tests/";
    int pathLen = strlen(pre) + strlen(cname) + 1;
    char *path = alloca(pathLen);
    sprintf(path, "%s%s", pre, cname);
    // printf("Path: %s\n", path);
    
    // open file
    FILE *f = fopen(path, "rb");
    assert(f != NULL);

    // get the length of the file
    fseek(f, 0, SEEK_END);
    long length = ftell(f);
    fseek(f, 0, SEEK_SET);

    // read file into buffer
    char *buffer = (char *)malloc(length + 1);
    fread(buffer, 1, length, f);
    fclose(f);
    buffer[length] = '\0';
    
    return buffer;
}

TEST(IntegrationTest, CompileFunctionLiteral){ 
    TEST_MODULE(loadTest("literal.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionBinaryInt){ 
    TEST_MODULE(loadTest("binaryInt.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionBinaryFloat){ 
    TEST_MODULE(loadTest("binaryFloat.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionLongVar){ 
    TEST_MODULE(loadTest("longVar.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionShortVar){ 
    TEST_MODULE(loadTest("shortVar.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionIf){ 
    TEST_MODULE(loadTest("if.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionIfElse){ 
    TEST_MODULE(loadTest("ifElse.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionIfElseIfElse){ 
    TEST_MODULE(loadTest("ifElseIfElse.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionIfElseIfElseIfElse){ 
    TEST_MODULE(loadTest("ifElseIfElseIfElse.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionFor){ 
    TEST_MODULE(loadTest("for.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionArrayInit){ 
    TEST_MODULE(loadTest("arrayInit.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionAdd){ 
    TEST_MODULE(loadTest("add.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionUnary){ 
    TEST_MODULE(loadTest("unary.acl"), 123); 
}

TEST(IntegrationTest, CompileFunctionReassignArg){ 
    TEST_MODULE(loadTest("reassignArg.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionGCD){ 
    TEST_MODULE(loadTest("gcd.acl"), 139);
}

TEST(IntegrationTest, CompileFunctionFibbonanci) {
    TEST_MODULE(loadTest("fibbonanci.acl"), 144);
}

TEST(IntegrationTest, CompileFunctionArraySum) {
    TEST_MODULE(loadTest("arraySum.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionNestedFor) {
    TEST_MODULE(loadTest("nestedFor.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionBubblesort) {
    TEST_MODULE(loadTest("bubblesort.acl"), 123);
}

TEST(IntegrationTest, CompileFunctionProcColonError) {
    TEST_ERROR(loadTest("procColonError.acl"), DOUBLE_COLON);
}

TEST(IntegrationTest, CompileFunctionProcArrowError) {
    TEST_ERROR(loadTest("procArrowError.acl"), ARROW);
}

TEST(IntegrationTest, CompileFunctionProcNameError) {
    TEST_ERROR(loadTest("procNameError.acl"), IDENT);
}

TEST(IntegrationTest, CompileFunctionVarEqualError) {
    TEST_ERROR(loadTest("varEqualError.acl"), ASSIGN);
}

TEST(IntegrationTest, CompileFunctionVarNameError) {
    TEST_ERROR(loadTest("varNameError.acl"), IDENT);
}