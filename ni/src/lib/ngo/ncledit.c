/*
 *      $Id: ncledit.c,v 1.3 1996-11-24 22:27:35 boote Exp $
 */
/************************************************************************
*									*
*			     Copyright (C)  1996			*
*	     University Corporation for Atmospheric Research		*
*			     All Rights Reserved			*
*									*
************************************************************************/
/*
 *	File:		ncledit.c
 *
 *	Author:		Jeff W. Boote
 *			National Center for Atmospheric Research
 *			PO 3000, Boulder, Colorado
 *
 *	Date:		Thu Sep 19 13:29:08 MDT 1996
 *
 *	Description:	This class isn't actually used yet.  It is a place
 *			holder so that as much common functionality can
 *			be pulled up from xncledit as possible - when
 *			we are actually forced to port to another arch.
 */
#include <ncarg/ngo/ncleditP.h>
#include <ncarg/ngo/nclstate.h>
#include <ncarg/ngo/xutil.h>
#include <ncarg/ngo/addfile.h>
#include <ncarg/ngo/load.h>

#include <Xm/Xm.h>
#include <Xm/Form.h>
#include <Xm/PushBG.h>
#include <Xm/SeparatoG.h>
#include <Xm/PanedW.h>
#include <Xm/PushB.h>
#include <Xm/Label.h>
#include <Xm/Frame.h>
#include <Xm/List.h>
#include <Xm/ScrolledW.h>
#include <Xm/Text.h>
#include <Xm/RowColumn.h>
#include <Xm/CascadeBG.h>
#include <Xm/MenuShell.h>

static char RSTSTR[] = "You must finish entering block, or press reset";
static char ERRSTR[] = "Error!";

#define	Oset(field)	NhlOffset(NgNclEditRec,ncledit.field)
static NhlResource resources[] = {
	{NgNneResetWarning,NgCneResetWarning,NhlTString,sizeof(NhlString),
		Oset(rmsg),NhlTImmediate,_NhlUSET((NhlPointer)RSTSTR),
		_NhlRES_RONLY,NULL},
	{NgNneErrorString,NgCneErrorString,NhlTString,sizeof(NhlString),
		Oset(emsg),NhlTImmediate,_NhlUSET((NhlPointer)ERRSTR),
		_NhlRES_RONLY,NULL},
};
#undef	Oset

static NhlErrorTypes NEInitialize(
	NhlClass	lc,
	NhlLayer	req,
	NhlLayer	new,
	_NhlArgList	args,
	int		nargs
);

static NhlErrorTypes NEDestroy(
	NhlLayer	l
);

static NhlBoolean NECreateWin(
	NgGO	go
);

NgNclEditClassRec NgnclEditClassRec = {
	{
/* class_name		*/	"nclEditClass",
/* nrm_class		*/	NrmNULLQUARK,
/* layer_size		*/	sizeof(NgNclEditRec),
/* class_inited		*/	False,
/* superclass		*/	(NhlClass)&NggOClassRec,
/* cvt_table		*/	NULL,

/* layer_resources	*/	resources,
/* num_resources	*/	NhlNumber(resources),
/* all_resources	*/	NULL,
/* callbacks		*/	NULL,
/* num_callbacks	*/	0,

/* class_part_initialize*/	NULL,
/* class_initialize	*/	NULL,
/* layer_initialize	*/	NEInitialize,
/* layer_set_values	*/	NULL,
/* layer_set_values_hook*/	NULL,
/* layer_get_values	*/	NULL,
/* layer_reparent	*/	NULL,
/* layer_destroy	*/	NEDestroy,

	},
	{
/* dialog		*/	NULL,
/* toplevel		*/	NULL,
/* manager		*/	NULL,

/* top_win_chain	*/	False,
/* create_win		*/	NECreateWin,
/* create_win_hook	*/	NULL,
	},
	{
/* foo			*/	0,
	},
};

NhlClass NgnclEditClass = (NhlClass)&NgnclEditClassRec;

/*
 * Function:	addFile
 *
 * Description:	
 *
 * In Args:	
 *
 * Out Args:	
 *
 * Scope:	
 * Returns:	
 * Side Effect:	
 */
static void
addFile
(
	Widget		w,
	XEvent		*xev,
	String		*params,
	Cardinal	*num_params
)
{
	char		func[] = "addFile";
	int		goid = NhlDEFAULT_APP;
	NgNclEdit	ncl = NULL;

	goid = NgGOWidgetToGoId(w);
	if(goid == NhlDEFAULT_APP){
		NHLPERROR((NhlFATAL,NhlEUNKNOWN,"%s:invalid Widget",func));
		return;
	}

	if((*num_params == 1) || (*num_params > 2)){
		NHLPERROR((NhlFATAL,NhlEUNKNOWN,
					"%s:wrong number of params",func));
		return;
	}
	else if(*num_params == 2){
		int		appmgr = NhlDEFAULT_APP;
		int		nclstate = NhlDEFAULT_APP;
		struct stat	buf;
		char		line[512];

		if((strlen(params[0])+strlen(params[1])) > (sizeof(line) - 19)){
			NHLPERROR((NhlFATAL,NhlEUNKNOWN,
				"%s:parameters too long:%s:%s",
				func,params[0],params[1]));
			return;
		}
		NhlVAGetValues(goid,
			_NhlNguiData,	&appmgr,
			NULL);
		NhlVAGetValues(appmgr,
			NgNappNclState,	&nclstate,
			NULL);

		if(!NhlIsClass(nclstate,NgnclStateClass)){
			NHLPERROR((NhlFATAL,NhlEUNKNOWN,
					"%s:invalid nclstate obj",func));
			return;
		}

		/*
		 * Check to see if file exists and is text...
		 */

		if(stat(params[1],&buf) != 0){
			NHLPERROR((NhlFATAL,errno,"%s:unable to access file %s",
							func,params[0]));
			return;
		}
		/*
		 * Submit it to nclstate.
		 */
		sprintf(line,"%s = addfile(\"%s\",\"r\")\n",
							params[0],params[1]);
		(void)NgNclSubmitBlock(nclstate,line);

		return;
	}

	ncl = (NgNclEdit)_NhlGetLayer(goid);
	if(!ncl || !_NhlIsClass((NhlLayer)ncl,NgnclEditClass)){
		NHLPERROR((NhlFATAL,NhlEUNKNOWN,"%s:invalid window",func));
		return;
	}

	/*
	 * Popup addfile selection box.
	 */
	if(ncl->ncledit.addfile == NhlDEFAULT_APP){
		NhlVACreate(&ncl->ncledit.addfile,"addfile",NgaddFileClass,goid,
			NULL);
	}
	NgGOPopup(ncl->ncledit.addfile);

	return;
}

/*
 * Function:	loadScript
 *
 * Description:	
 *
 * In Args:	
 *
 * Out Args:	
 *
 * Scope:	
 * Returns:	
 * Side Effect:	
 */
static void
loadScript
(
	Widget		w,
	XEvent		*xev,
	String		*params,
	Cardinal	*num_params
)
{
	char		func[] = "loadScript";
	int		goid = NhlDEFAULT_APP;
	NgNclEdit	ncl = NULL;

	goid = NgGOWidgetToGoId(w);
	if(goid == NhlDEFAULT_APP){
		NHLPERROR((NhlFATAL,NhlEUNKNOWN,"%s:invalid Widget",func));
		return;
	}

	if(*num_params > 1){
		NHLPERROR((NhlFATAL,NhlEUNKNOWN,
					"%s:wrong number of params",func));
		return;
	}
	else if(*num_params == 1){
		int		appmgr = NhlDEFAULT_APP;
		int		nclstate = NhlDEFAULT_APP;
		struct stat	buf;
		char		line[512];

		if(strlen(params[0]) > (sizeof(line) - 9)){
			NHLPERROR((NhlFATAL,NhlEUNKNOWN,
				"%s:parameter too long:%s",func,params[0]));
			return;
		}
		NhlVAGetValues(goid,
			_NhlNguiData,	&appmgr,
			NULL);
		NhlVAGetValues(appmgr,
			NgNappNclState,	&nclstate,
			NULL);

		if(!NhlIsClass(nclstate,NgnclStateClass)){
			NHLPERROR((NhlFATAL,NhlEUNKNOWN,
					"%s:invalid nclstate obj",func));
			return;
		}

		/*
		 * Check to see if file exists and is text...
		 */

		if(stat(params[0],&buf) != 0){
			NHLPERROR((NhlFATAL,errno,"%s:unable to access file %s",
							func,params[0]));
			return;
		}
		/*
		 * Submit it to nclstate.
		 */
		sprintf(line,"load \"%s\"\n",params[0]);
		(void)NgNclSubmitBlock(nclstate,line);

		return;
	}

	ncl = (NgNclEdit)_NhlGetLayer(goid);
	if(!ncl || !_NhlIsClass((NhlLayer)ncl,NgnclEditClass)){
		NHLPERROR((NhlFATAL,NhlEUNKNOWN,"%s:invalid window",func));
		return;
	}

	/*
	 * Popup load file selection box.
	 */
	if(ncl->ncledit.load == NhlDEFAULT_APP){
		NhlVACreate(&ncl->ncledit.load,"load",NgloadClass,goid,
			NULL);
	}
	NgGOPopup(ncl->ncledit.load);

	return;
}

static XtActionsRec ncl_act[] = {
	{"addFile", addFile,},
	{"loadScript", loadScript,},
};

static NhlErrorTypes
NEInitialize
(
	NhlClass	lc,
	NhlLayer	req,
	NhlLayer	new,
	_NhlArgList	args,
	int		nargs
)
{
	static NhlBoolean	init = True;
	char			func[] = "NEInitialize";
	NgNclEdit		ncl = (NgNclEdit)new;
	NgNclEditPart		*np = &((NgNclEdit)new)->ncledit;
	NgNclEditPart		*rp = &((NgNclEdit)req)->ncledit;

	np->nsid = NhlDEFAULT_APP;
	NhlVAGetValues(ncl->go.appmgr,
		NgNappNclState,	&np->nsid,
		NULL);
	if(!NhlIsClass(np->nsid,NgnclStateClass)){
		NHLPERROR((NhlFATAL,NhlEUNKNOWN,"%s:Invalid nclstate resource",
									func));
		return NhlFATAL;
	}

	if(init){
		init = False;
		XtAppAddActions(ncl->go.x->app_con,ncl_act,NhlNumber(ncl_act));
	}

	np->rstr = NgXAppCreateXmString(ncl->go.appmgr,np->rmsg);
	np->rmsg = NULL;
	np->estr = NgXAppCreateXmString(ncl->go.appmgr,np->emsg);
	np->emsg = NULL;
	np->nstr = NgXAppCreateXmString(ncl->go.appmgr,"");
	np->cstr = np->nstr;

	np->edit = False;
	np->my_cmd = False;
	np->print = False;
	np->more_cmds = NULL;
	np->prompt_pos = np->submit_pos = np->reset_pos = 0;
	np->line = 0;
	np->my_focus = False;

	np->load = NhlDEFAULT_APP;
	np->addfile = NhlDEFAULT_APP;

	return NhlNOERROR;
}

static NhlErrorTypes
NEDestroy
(
	NhlLayer	l
)
{
	NgNclEdit	ncl = (NgNclEdit)l;
	NgNclEditPart	*np = &((NgNclEdit)l)->ncledit;

	/*
	 * Don't destroy widgets!  NgGO takes care of that.
	 */
	NgXAppFreeXmString(ncl->go.appmgr,np->rstr);
	np->rstr = NULL;
	NgXAppFreeXmString(ncl->go.appmgr,np->estr);
	np->estr = NULL;
	NgXAppFreeXmString(ncl->go.appmgr,np->nstr);
	np->nstr = NULL;
	np->cstr = NULL;

	if(np->more_cmds){
		NhlFree(np->more_cmds);
	}

	if(np->load != NhlDEFAULT_APP){
		NhlDestroy(np->load);
		np->load = NhlDEFAULT_APP;
	}

	return NhlNOERROR;
}

static void Popup
(
	Widget		w,
	XtPointer	udata,
	XtPointer	cbdata
)
{
	XmProcessTraversal((Widget)udata,XmTRAVERSE_CURRENT);
}

static void
CheckInput
(
	Widget		w,
	XtPointer	udata,
	XtPointer	cbdata
)
{
	NgNclEdit	ncl = (NgNclEdit)udata;
	NgNclEditPart	*np = &ncl->ncledit;
	XmTextVerifyPtr	ver = (XmTextVerifyPtr)cbdata;
	char		*nl,*ptr;
	int		oset,len;

	if(np->edit)
		return;

	if(ver->reason != XmCR_MODIFYING_TEXT_VALUE)
		return;

	/*
	 * Don't allow things before submit_pos to be modified.
	 */
	if(ver->startPos < np->submit_pos){
		if(ver->endPos < np->submit_pos){
			/*
			 * The insert cursor is completely before the
			 * submit position.  Just take the text being
			 * typed or pasted and put it at the end.
			 */
			ver->startPos = ver->endPos = XmTextGetLastPosition(w);
		}
		else{
			/*
			 * The endPos was in the valid edit area, so just
			 * move the startPos up to submit position.
			 * If start == end and the text has 0 length,
			 * then the user is trying to backspace over the
			 * prompt, so I disallow that.
			 */
			ver->startPos = np->submit_pos;
			if(ver->startPos == ver->endPos &&
							ver->text->length == 0){
				ver->doit = False;
				return;
			}
		}
	}

	if(ver->text->length <= 1)
		return;

	/*
	 * Find first unescaped newline.
	 */
	nl = strchr(ver->text->ptr,'\n');
	while(nl && nl != ver->text->ptr && *(nl - 1) == '\\')
		nl = strchr(++nl,'\n');

	if(!nl)
		return;

	/*
	 * take first line of input and send it through
	 */
	oset = nl - ver->text->ptr + 1;
	ptr = NhlMalloc(sizeof(char)*(oset + 1));
	if(!ptr){
		NHLPERROR((NhlFATAL,ENOMEM,NULL));
		ver->doit = False;
		return;
	}
	strncpy(ptr,ver->text->ptr,oset);
	ptr[oset] = '\0';
	ver->text->ptr = ptr;
	len = ver->text->length - oset;
	ver->text->length = oset;

	/*
	 * take remaining input and put it in ncledit structure for processing
	 * during activate() action.
	 */
	np->more_cmds = NhlMalloc(sizeof(char)*(len + 1));
	if(!np->more_cmds){
		NHLPERROR((NhlFATAL,ENOMEM,NULL));
		return;
	}
	nl++;
	strcpy(np->more_cmds,nl);

	return;
}

static void
ActivateCB
(
	Widget		w,
	XtPointer	udata,
	XtPointer	cbdata
)
{
	char		func[] = "ActivateCB";
	NgNclEdit	ncl = (NgNclEdit)udata;
	NgNclEditPart	*np = &ncl->ncledit;
	char		*cmd_stack[128];
	char		*cmd_buff;
	char		*nl;
	char		*cmd;
	int		len;

	if(np->edit)
		return;
	np->edit = True;

	len = XmTextGetLastPosition(w) - np->submit_pos;
	if(len < 1){
		np->edit = False;
		return;
	}

	cmd_buff = _NgStackAlloc(len + 1,cmd_stack);
	if(XmTextGetSubstring(w,np->submit_pos,len,len+1,cmd_buff)
							!= XmCOPY_SUCCEEDED){
		NHLPERROR((NhlFATAL,NhlEUNKNOWN,"%s:Unable to retrive string.",
									func));
		goto FREE_MEM;
	}

	/*
	 * Find first unescaped newline.
	 */
	nl = strchr(cmd_buff,'\n');
	while(nl && nl != cmd_buff && *(nl - 1) == '\\')
		nl = strchr(++nl,'\n');

	if(!nl){
		if(np->more_cmds){
			NHLPERROR((NhlFATAL,NhlEUNKNOWN,
						"%s:NclEdit BAD STATE!",func));
		}
		goto FREE_MEM;
	}

	if(nl != &cmd_buff[len-1]){
		NHLPERROR((NhlFATAL,NhlEUNKNOWN,"%s:NclEdit BAD STATE!",func));
		goto FREE_MEM;
	}

	np->my_cmd = True;

	if(!NgNclSubmitLine(np->nsid,cmd_buff,False))
		goto FREE_MEM;

	np->my_cmd = False;
	cmd = np->more_cmds;

	while(cmd && *cmd){
		/*
		 * If there is a terminating newline for cmd, then submit
		 * it, otherwise, just paste it on the end of submittext.
		 */
		nl = strchr(cmd,'\n');
		while(nl && nl > cmd && *(nl-1) == '\\')
			nl = strchr(++nl,'\n');

		if(!nl){
			XmTextInsert(w,XmTextGetLastPosition(w),cmd);
			break;
		}

		if(!NgNclSubmitLine(np->nsid,cmd,False))
			break;

		cmd = nl + 1;
	}


FREE_MEM:
	_NgStackFree(cmd_buff,cmd_stack);
	np->edit = False;
	np->my_cmd = False;
	if(np->more_cmds) NhlFree(np->more_cmds);

	return;
}


static void
SubmitCB
(
	NhlArgVal	cbdata,
	NhlArgVal	udata
)
{
	NgNclEdit	ncl = (NgNclEdit)udata.ptrval;
	NgNclEditPart	*np = &ncl->ncledit;
	NhlBoolean	edit;

	if(np->my_cmd) return;
	edit = np->edit;
	np->edit = True;

	XmTextReplace(np->text,np->submit_pos,XmTextGetLastPosition(np->text),
								cbdata.strval);
	np->edit = edit;

	return;
}

static void
OutputCB
(
	NhlArgVal	cbdata,
	NhlArgVal	udata
)
{
	NgNclEdit	ncl = (NgNclEdit)udata.ptrval;
	NgNclEditPart	*np = &ncl->ncledit;
	NhlBoolean	edit;

	edit = np->edit;
	np->edit = True;

	np->print = True;
	XmTextInsert(np->text,XmTextGetLastPosition(np->text),cbdata.strval);
	np->edit = edit;

	return;
}

static void
ErrOutputCB
(
	NhlArgVal	cbdata,
	NhlArgVal	udata
)
{
	NgNclEdit	ncl = (NgNclEdit)udata.ptrval;
	NgNclEditPart	*np = &ncl->ncledit;
	NhlBoolean	edit;
	char		buffer[NhlERRMAXMSGLEN+1];
	int		len;

	NhlErrSPrintMsg(buffer,(NhlErrMsg*)cbdata.ptrval);
	len = strlen(buffer);
	if(len < 1)
		return;
	if(buffer[len-1] != '\n')
		strcat(buffer,"\n");

	edit = np->edit;
	np->edit = True;
	XmTextInsert(np->text,XmTextGetLastPosition(np->text),buffer);
	np->edit = edit;

	return;
}

static void
PromptCB
(
	NhlArgVal	cbdata,
	NhlArgVal	udata
)
{
	char			func[] = "PromptCB";
	NgNclEdit		ncl = (NgNclEdit)udata.ptrval;
	NgNclEditPart		*np = &ncl->ncledit;
	NhlBoolean		edit;
	XmTextPosition		pos = XmTextGetLastPosition(np->text);
	char			buff[20];
	NgNclPromptCBData	prompt = (NgNclPromptCBData)cbdata.ptrval;
	XmString		msg;

	edit = np->edit;
	np->edit = True;

	if(XmTextGetSubstring(np->text,pos-2,2,3,buff) != XmCOPY_SUCCEEDED){
		NHLPERROR((NhlFATAL,NhlEUNKNOWN,"%s:Can't get text.",func));
	}
	else if(np->print){
		/*
		 * If NclOutput was called put an extra blank line in before
		 * prompt.
		 */
		np->print = False;
		if(buff[1] != '\n'){
			XmTextInsert(np->text,pos,"\n\n");
			pos += 2;
		}
		else if(buff[0] != '\n')
			XmTextInsert(np->text,pos++,"\n");
	}
	else if(buff[1] != '\n')
		XmTextInsert(np->text,pos++,"\n");

	np->prompt_pos = pos;
	if(prompt->istate){
		np->reset_pos = pos;
		if(np->my_focus){
			XtSetSensitive(np->reset,False);
			NgAppReleaseFocus(ncl->go.appmgr,ncl->base.id);
			np->my_focus = False;
		}
		np->cstr = np->nstr;
	}
	else{
		if(edit && !np->my_focus){
			XtSetSensitive(np->reset,True);
			NgAppGrabFocus(ncl->go.appmgr,ncl->base.id);
			np->my_focus = True;
			np->cstr = np->rstr;
		}
	}

	msg = np->cstr;

	if(prompt->err){
		XBell(ncl->go.x->dpy,0);
		NgGOPopup(ncl->base.id);
		msg = np->estr;
	}

	XtVaSetValues(np->ilabel,
		XmNlabelString,	msg,
		NULL);

	sprintf(buff,"ncl %d> ",prompt->line);
	XmTextInsert(np->text,pos,buff);
	np->submit_pos = pos + strlen(buff);
	np->line = prompt->line;

	np->edit = edit;

	return;
}

static void
ResetCB
(
	NhlArgVal	cbdata,
	NhlArgVal	udata
)
{
	NgNclEdit		ncl = (NgNclEdit)udata.ptrval;
	NgNclEditPart		*np = &ncl->ncledit;

	XmTextSetHighlight(np->text,np->reset_pos,np->prompt_pos,
						XmHIGHLIGHT_SECONDARY_SELECTED);
	np->reset_pos = np->prompt_pos;
	if(np->my_focus){
		XtSetSensitive(np->reset,False);
		NgAppReleaseFocus(ncl->go.appmgr,ncl->base.id);
		np->cstr = np->nstr;
		XtVaSetValues(np->ilabel,
			XmNlabelString,	np->cstr,
			NULL);
		np->my_focus = False;
	}

	return;
}

static void ResetButtonCB
(
	Widget		w,
	XtPointer	udata,
	XtPointer	cbdata
)
{
	NgNclReset((int)udata);
}

static NhlBoolean
NECreateWin
(
	NgGO	go
)
{
	char		func[]="NECreateWin";
	NgNclEditPart	*np = &((NgNclEdit)go)->ncledit;
	Widget		menubar,menush,fmenu,fmenush,emenu,emenush;
	Widget		cmenu,cmenush;
	Widget		hmenu,hmenush;
	Widget		file,edit,config,help;
	Widget		addfile,load,close,quit;
	Widget		pane,sform,sform1;
	Widget		slabel;
	Widget		hoframe,holabel;
	Widget		vframe,vlabel;
	Widget		fframe,flabel;
	Widget		fuframe,fulabel;
	Widget		oform,olabel;
	Widget		iform,scroll;
	Dimension	width;
	Arg		args[10];
	int		nargs;
	NhlLayer	nstate;
	NhlArgVal	dummy,udata;
	char		buff[20];

	menubar =XtVaCreateManagedWidget("menubar",xmRowColumnWidgetClass,
								go->go.manager,
		XmNrowColumnType,	XmMENU_BAR,
		NULL);

	menush = XtVaCreatePopupShell("menush",xmMenuShellWidgetClass,
								go->go.shell,
		XmNwidth,		5,
		XmNheight,		5,
		XmNallowShellResize,	True,
		XtNoverrideRedirect,	True,
		NULL);
	fmenu = XtVaCreateWidget("fmenu",xmRowColumnWidgetClass,menush,
		XmNrowColumnType,	XmMENU_PULLDOWN,
		NULL);

	emenu = XtVaCreateWidget("emenu",xmRowColumnWidgetClass,menush,
		XmNrowColumnType,	XmMENU_PULLDOWN,
		NULL);

	cmenu = XtVaCreateWidget("cmenu",xmRowColumnWidgetClass,menush,
		XmNrowColumnType,	XmMENU_PULLDOWN,
		NULL);

	hmenu = XtVaCreateWidget("hmenu",xmRowColumnWidgetClass,menush,
		XmNrowColumnType,	XmMENU_PULLDOWN,
		NULL);

	file = XtVaCreateManagedWidget("file",xmCascadeButtonGadgetClass,
									menubar,
		XmNsubMenuId,	fmenu,
		NULL);

	edit = XtVaCreateManagedWidget("edit",xmCascadeButtonGadgetClass,
									menubar,
		XmNsubMenuId,	emenu,
		NULL);

	config = XtVaCreateManagedWidget("config",xmCascadeButtonGadgetClass,
									menubar,
		XmNsubMenuId,	cmenu,
		NULL);

	help = XtVaCreateManagedWidget("help",xmCascadeButtonGadgetClass,
									menubar,
		XmNsubMenuId,	hmenu,
		NULL);

	XtVaSetValues(menubar,
		XmNmenuHelpWidget,	help,
		NULL);

	addfile = XtVaCreateManagedWidget("addFile",
					xmPushButtonGadgetClass,fmenu,
		NULL);
	XtAddCallback(addfile,XmNactivateCallback,_NgGODefActionCB,NULL);

	load = XtVaCreateManagedWidget("loadScript",
					xmPushButtonGadgetClass,fmenu,
		NULL);
	XtAddCallback(load,XmNactivateCallback,_NgGODefActionCB,NULL);

	close = XtVaCreateManagedWidget("closeWindow",
					xmPushButtonGadgetClass,fmenu,
		NULL);
	XtAddCallback(close,XmNactivateCallback,_NgGODefActionCB,NULL);

	quit = XtVaCreateManagedWidget("quitApplication",
					xmPushButtonGadgetClass,fmenu,
		NULL);
	XtAddCallback(quit,XmNactivateCallback,_NgGODefActionCB,NULL);

	XtManageChild(fmenu);
	XtManageChild(emenu);
	XtManageChild(cmenu);
	XtManageChild(hmenu);

	pane = XtVaCreateManagedWidget("pane",xmPanedWindowWidgetClass,
								go->go.manager,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNtopWidget,		menubar,
		NULL);

	sform = XtVaCreateManagedWidget("sform",xmFormWidgetClass,pane,
		NULL);

	slabel = XtVaCreateManagedWidget("slabel",xmLabelWidgetClass,sform,
		NULL);

	hoframe = XtVaCreateManagedWidget("hoframe",xmFrameWidgetClass,sform,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNtopWidget,		slabel,
		XmNrightAttachment,	XmATTACH_POSITION,
		NULL);

	holabel = XtVaCreateManagedWidget("holabel",xmLabelWidgetClass,hoframe,
		XmNchildType,			XmFRAME_TITLE_CHILD,
		NULL);

	nargs = 0;
	np->holist = XmCreateScrolledList(hoframe,"holist",args,nargs);
	XtManageChild(np->holist);

	(void)NgXListManage(np->nsid,np->holist,NgNclHLUVAR,NULL,NULL);

	vframe = XtVaCreateManagedWidget("vframe",xmFrameWidgetClass,sform,
		XmNtopWidget,		slabel,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNleftAttachment,	XmATTACH_POSITION,
		XmNrightAttachment,	XmATTACH_POSITION,
		NULL);

	vlabel = XtVaCreateManagedWidget("vlabel",xmLabelWidgetClass,vframe,
		XmNchildType,			XmFRAME_TITLE_CHILD,
		NULL);

	np->vlist = XmCreateScrolledList(vframe,"vlist",NULL,0);
	XtManageChild(np->vlist);

	(void)NgXListManage(np->nsid,np->vlist,NgNclVAR,NULL,NULL);

	fframe = XtVaCreateManagedWidget("fframe",xmFrameWidgetClass,sform,
		XmNtopWidget,		slabel,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNleftAttachment,	XmATTACH_POSITION,
		XmNrightAttachment,	XmATTACH_POSITION,
		NULL);

	flabel = XtVaCreateManagedWidget("flabel",xmLabelWidgetClass,fframe,
		XmNchildType,			XmFRAME_TITLE_CHILD,
		NULL);

	np->flist = XmCreateScrolledList(fframe,"flist",NULL,0);
	XtManageChild(np->flist);

	(void)NgXListManage(np->nsid,np->flist,NgNclFILEVAR,NULL,NULL);

	fuframe = XtVaCreateManagedWidget("fuframe",xmFrameWidgetClass,sform,
		XmNtopWidget,		slabel,
		XmNtopAttachment,	XmATTACH_WIDGET,
		XmNleftAttachment,	XmATTACH_POSITION,
		NULL);

	fulabel = XtVaCreateManagedWidget("fulabel",xmLabelWidgetClass,fuframe,
		XmNchildType,			XmFRAME_TITLE_CHILD,
		NULL);

	nargs = 0;
	np->fulist = XmCreateScrolledList(fuframe,"fulist",NULL,0);
	XtManageChild(np->fulist);

	(void)NgXListManage(np->nsid,np->fulist,NgNclFUNC,NULL,NULL);

	iform = XtVaCreateManagedWidget("iform",xmFormWidgetClass,pane,
		NULL);

	np->reset = XtVaCreateManagedWidget("reset",xmPushButtonWidgetClass,
									iform,
		NULL);
	XtAddCallback(np->reset,XmNactivateCallback,ResetButtonCB,
							(XtPointer)np->nsid);

	np->ilabel = XtVaCreateManagedWidget("ilabel",xmLabelWidgetClass,iform,
		XmNleftAttachment,	XmATTACH_WIDGET,
		XmNleftWidget,		np->reset,
		XmNtopAttachment,	XmATTACH_OPPOSITE_WIDGET,
		XmNtopWidget,		np->reset,
		NULL);

	scroll = XtVaCreateManagedWidget("scroll",xmScrolledWindowWidgetClass,
									iform,
		XmNbottomAttachment,	XmATTACH_WIDGET,
		XmNbottomWidget,	np->reset,
		NULL);

	np->text = XtVaCreateManagedWidget("nclcmd",xmTextWidgetClass,scroll,
		XmNeditMode,	XmMULTI_LINE_EDIT,
		NULL);
	XtAddCallback(np->text,XmNmodifyVerifyCallback,CheckInput,
								(XtPointer)go);
	XtAddCallback(np->text,XmNactivateCallback,ActivateCB,(XtPointer)go);

	XtAddCallback(go->go.shell,XmNpopupCallback,Popup,(XtPointer)np->text);

#ifdef	DEBUG
	memset(&dummy,0,sizeof(NhlArgVal));
	memset(&udata,0,sizeof(NhlArgVal));
#endif
	udata.ptrval = go;

	nstate = _NhlGetLayer(np->nsid);
	np->submitcb = _NhlAddObjCallback(nstate,NgCBnsSubmit,dummy,SubmitCB,
									udata);
	np->promptcb = _NhlAddObjCallback(nstate,NgCBnsPrompt,dummy,PromptCB,
									udata);
	np->resetcb = _NhlAddObjCallback(nstate,NgCBnsReset,dummy,ResetCB,
									udata);
	np->outputcb = _NhlAddObjCallback(nstate,NgCBnsOutput,dummy,OutputCB,
									udata);
	np->erroutputcb = _NhlAddObjCallback(nstate,NgCBnsErrOutput,dummy,
							ErrOutputCB,udata);

	np->line = NgNclCurrentLine(np->nsid);
	sprintf(buff,"ncl %d> ",np->line);
	np->edit = True;
	XmTextReplace(np->text,0,XmTextGetLastPosition(np->text),buff);
	np->edit = False;
	np->reset_pos = 0;
	np->submit_pos = np->reset_pos + strlen(buff);

	return True;
}
