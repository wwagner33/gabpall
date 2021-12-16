/*
 ****************************************************************
 *                                                              *
 *      Module: fnc.h                                           *
 *      Description: Declaration of Global Functiions           *
 *      Version: 1.0 of 06/Jun/95                               *
 *      Author: Simon Gay and Juarez Muylaert                   *
 *                                                              *
 ****************************************************************
 */

EXPRESSION *MakeNumNode ();
EXPRESSION *MakeBoolNode ();
EXPRESSION *MakeIdentNode ();
EXPRESSION *MakeBinNode ();
ACTION *MakeActNode ();
ACTION *LinkActNodes ();
PATTERN *MakePatNode ();
PATTERN *LinkPatNodes ();
EXPRESSION *MakeTupleNode ();
BODY *MakeBasicBodyNode ();
BODY *MakeIdentBodyNode ();
BODY *MakeCombBodyNode ();
DEFINITION *MakeDefNode ();
DEFS *MakeSingleDefNode ();
DEFS *LinkDefNodes ();
BAG *MakeSingleBagNode ();
BAG *MakeTupleBagNode ();
BAG *LinkBagNodes ();
TUPLE *MakeSingleTupleBagNode ();
TUPLE *LinkTupleNodes ();
PROGRAM *MakeProgramNode ();
EXPRESSION MakeEmptyExpression ();
ACTION *MakeEmptyAction ();
void PrintNumNode ();
void PrintBoolNode ();
void PrintIdentNode ();
void PrintExpression ();
void PrintListDefinitions ();
void PrintBody ();
void PrintBasicBody ();
void PrintCombinatorBody ();
void PrintPattern ();
void PrintAction ();
void PrintBag ();
void PrintTuple ();
char *Alloc ();
void Free ();
RT_BAG *CreateInitialBagNode ();
void DeleteRTBagNode ();
RT_BAG *CreatRTBagNode ();
RT_BAG *AddRTBagNode ();
RT_BAG *AddRTTupleBagNode ();
RT_BAG *SearchRTBagInteger ();
void UnlockBagNodes ();
RT_BAG *SearchTuple ();
int TuplesMatch ();
RT_TUPLE *GenTupleSkeleton ();
NAMELIST *NamesInExpression ();

void CodePrintNumNode ();
void CodePrintBoolNode ();
void CodePrintIdentNode ();
void CodePrintExpression ();
void CodePrintListDefinitions ();
void CodePrintBody ();
void CodePrintBasicBody ();
void CodePrintCombinatorBody ();
void CodePrintPattern ();
void CodePrintAction ();
void CodePrintBag ();
void CodePrintTuple ();
void CodePrintProgram ();

void WriteHeader ();
void CodeInitialBag ();
void GenBasicExpression ();
int GetTupleSize ();
void GenTuple ();
void GenCodeExpression ();
void GenCodeReaction ();
void GenCodeBagIntSearch ();

void PrintProgram ();
int check_names_in_program();
int check_multiple_defs();
int check_names_in_program();
int CheckPatternsInDefinitions ();
int CheckNamesInProgram();
void TranslateProgramPatterns();
void GenScript();
int GenMainCode();
int IsElement();

