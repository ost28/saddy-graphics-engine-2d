/*! \file jspipelinestep.h
 
    A simple pipeline steps for performingcompiled function calls
 */
#pragma once
#include "../pipeline/pipelinestep.h"
#include "../timer.h"
#include "context.h"

namespace sad
{
    
namespace dukpp03
{

/*! A type of pipeline step type
 */    
enum JSPipelineStepType
{
    SDJST_EACH_FRAME,  //!< En event, that performed each frame
    SDJST_EACH_MS,     //!< A repeated event, which repeats every N ms
    SDJST_ONE_SHOT,    //!< A one-shot event
    SDJST_DELAYED      //!< A delayed event
};

/*! Pipeline step type have several policies
 */
template<
    JSPipelineStepType Type
>
class JSPipelineStep
{
    
};

/*! An action, that will be performed each frame
 */
template<>
class JSPipelineStep<sad::dukpp03::SDJST_EACH_FRAME>: public sad::pipeline::Step
{
public:
    /*! Makes new handler
        \param[in] ctx context
        \param[in] f function
     */
    JSPipelineStep(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Destroys a step
     */
    virtual ~JSPipelineStep();
    /*! Determines, whether after processing we should remove step from pipeline
        \return whether we should remove step from pipeline
     */
    virtual bool shouldBeDestroyedAfterProcessing(); 
protected:
    /*! Processes the event
     */
    virtual void _process();
    /*! A context
     */
    sad::dukpp03::Context* m_ctx;
    /*! A function
     */
    sad::dukpp03::CompiledFunction m_function;
};



/*! An action, that will be performed each frame
 */
template<>
class JSPipelineStep<sad::dukpp03::SDJST_EACH_MS>: public sad::pipeline::Step
{
public:
    /*! Makes new handler
        \param[in] interval an interval for launch
        \param[in] ctx context
        \param[in] f function
     */
    JSPipelineStep(double interval, sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Destroys a step
     */
    virtual ~JSPipelineStep();
    /*! Determines, whether after processing we should remove step from pipeline
        \return whether we should remove step from pipeline
     */
    virtual bool shouldBeDestroyedAfterProcessing(); 
protected:
    /*! Processes the event
     */
    virtual void _process();
    /*! A timer for measuring time
     */
    sad::Timer m_timer;
    /*! An interval for performing action
     */
    double m_interval;
    /*! A context
     */
    sad::dukpp03::Context* m_ctx;
    /*! A function
     */
    sad::dukpp03::CompiledFunction m_function;
};


/*! An action, that will be performed each frame
 */
template<>
class JSPipelineStep<sad::dukpp03::SDJST_ONE_SHOT>: public sad::pipeline::Step
{
public:
    /*! Makes new handler
        \param[in] ctx context
        \param[in] f function
     */
    JSPipelineStep(sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Destroys a step
     */
    virtual ~JSPipelineStep();
    /*! Determines, whether after processing we should remove step from pipeline
        \return whether we should remove step from pipeline
     */
    virtual bool shouldBeDestroyedAfterProcessing(); 
protected:
    /*! Processes the event
     */
    virtual void _process();
    /*! A context
     */
    sad::dukpp03::Context* m_ctx;
    /*! A function
     */
    sad::dukpp03::CompiledFunction m_function;
    /*! Whether we should remove this stape
     */
    bool m_should_be_removed;
};



/*! An action, that will be performed each frame
 */
template<>
class JSPipelineStep<sad::dukpp03::SDJST_DELAYED>: public sad::pipeline::Step
{
public:
    /*! Makes new handler
        \param[in] interval an interval for launch
        \param[in] ctx context
        \param[in] f function
     */
    JSPipelineStep(double interval, sad::dukpp03::Context* ctx, sad::dukpp03::CompiledFunction f);
    /*! Destroys a step
     */
    virtual ~JSPipelineStep();
    /*! Determines, whether after processing we should remove step from pipeline
        \return whether we should remove step from pipeline
     */
    virtual bool shouldBeDestroyedAfterProcessing(); 
protected:
    /*! Processes the event
     */
    virtual void _process();
    /*! A timer for measuring time
     */
    sad::Timer m_timer;
    /*! An interval for performing action
     */
    double m_interval;
    /*! A context
     */
    sad::dukpp03::Context* m_ctx;
    /*! A function
     */
    sad::dukpp03::CompiledFunction m_function;
    /*! Whether we should remove this stape
     */
    bool m_should_be_removed;
};

}

}
