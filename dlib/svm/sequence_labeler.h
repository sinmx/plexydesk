// Copyright (C) 2011  Davis E. King (davis@dlib.net)
// License: Boost Software License   See LICENSE.txt for the full license.
#ifndef DLIB_SEQUENCE_LAbELER_H___
#define DLIB_SEQUENCE_LAbELER_H___

#include "sequence_labeler_abstract.h"
#include "../matrix.h"
#include <vector>
#include "../optimization/find_max_factor_graph_viterbi.h"

namespace dlib
{

// ----------------------------------------------------------------------------------------

    namespace fe_helpers
    {
        template <typename EXP>
        struct dot_functor
        {
            dot_functor(const matrix_exp<EXP>& lambda_) : lambda(lambda_), value(0) {}

            inline void operator() (
                unsigned long feat_index
            )
            {
                value += lambda(feat_index);
            }

            inline void operator() (
                unsigned long feat_index,
                double feat_value
            )
            {
                value += feat_value*lambda(feat_index);
            }

            const matrix_exp<EXP>& lambda;
            double value;
        };

        template <typename feature_extractor, typename EXP, typename sample_type, typename EXP2> 
        double dot(
            const matrix_exp<EXP>& lambda,
            const feature_extractor& fe,
            const std::vector<sample_type>& sequence,
            const matrix_exp<EXP2>& candidate_labeling,
            unsigned long position
        )
        {
            dot_functor<EXP> dot(lambda);
            fe.get_features(dot, sequence, candidate_labeling, position);
            return dot.value;
        }

    }

// ----------------------------------------------------------------------------------------

    namespace impl
    {
        template <
            typename T,
            bool (T::*funct)(const std::vector<typename T::sample_type>&, const matrix_exp<matrix<unsigned long> >&, unsigned long)const
            >
        struct hrlh_helper
        {
            typedef char type;
        };

        template <typename T>
        char has_reject_labeling_helper( typename hrlh_helper<T,&T::template reject_labeling<matrix<unsigned long> > >::type = 0 ) { return 0;}

        struct two_bytes
        {
            char a[2]; 
        };

        template <typename T>
        two_bytes has_reject_labeling_helper(int) { return two_bytes();}

        template <typename T>
        struct work_around_visual_studio_bug
        {
            const static unsigned long U = sizeof(has_reject_labeling_helper<T>('a'));
        };


        // This is a template to tell you if a feature_extractor has a reject_labeling function or not.
        template <typename T, unsigned long U = work_around_visual_studio_bug<T>::U > 
        struct has_reject_labeling 
        {
            static const bool value = false;
        };

        template <typename T>
        struct has_reject_labeling <T,1>
        {
            static const bool value = true;
        };


        template <typename feature_extractor, typename EXP, typename sample_type>
        typename enable_if<has_reject_labeling<feature_extractor>,bool>::type call_reject_labeling_if_exists (
            const feature_extractor& fe,
            const std::vector<sample_type>& x,
            const matrix_exp<EXP>& y,
            unsigned long position
        )
        {
            return fe.reject_labeling(x, y, position);
        }

        template <typename feature_extractor, typename EXP, typename sample_type>
        typename disable_if<has_reject_labeling<feature_extractor>,bool>::type call_reject_labeling_if_exists (
            const feature_extractor& ,
            const std::vector<sample_type>& ,
            const matrix_exp<EXP>& ,
            unsigned long 
        )
        {
            return false;
        }
    }


// ----------------------------------------------------------------------------------------

    template <
        typename feature_extractor
        >
    class sequence_labeler
    {
    public:
        typedef typename feature_extractor::sample_type sample_type;
        typedef std::vector<sample_type> sample_sequence_type;
        typedef std::vector<unsigned long> labeled_sequence_type;

    private:
        class map_prob
        {
        public:
            unsigned long order() const { return fe.order(); }
            unsigned long num_states() const { return fe.num_labels(); }

            map_prob(
                const sample_sequence_type& x_,
                const feature_extractor& fe_,
                const matrix<double,0,1>& weights_
            ) :
                sequence(x_),
                fe(fe_),
                weights(weights_)
            {
            }

            unsigned long number_of_nodes(
            ) const
            {
                return sequence.size();
            }

            template <
                typename EXP 
                >
            double factor_value (
                unsigned long node_id,
                const matrix_exp<EXP>& node_states
            ) const
            {
                if (dlib::impl::call_reject_labeling_if_exists(fe, sequence,  node_states, node_id))
                    return -std::numeric_limits<double>::infinity();

                return fe_helpers::dot(weights, fe, sequence, node_states, node_id);
            }

            const sample_sequence_type& sequence;
            const feature_extractor& fe;
            const matrix<double,0,1>& weights;
        };
    public:

        sequence_labeler()
        {
            weights.set_size(fe.num_features());
            weights = 0;
        }

        sequence_labeler(
            const feature_extractor& fe_,
            const matrix<double,0,1>& weights_
        ) :
            fe(fe_),
            weights(weights_)
        {
            // make sure requires clause is not broken
            DLIB_ASSERT(fe_.num_features() == static_cast<unsigned long>(weights_.size()),
                "\t sequence_labeler::sequence_labeler()"
                << "\n\t These sizes should match"
                << "\n\t fe_.num_features(): " << fe_.num_features() 
                << "\n\t weights_.size():    " << weights_.size() 
                << "\n\t this: " << this
                );
        }

        const feature_extractor& get_feature_extractor (
        ) const { return fe; }

        const matrix<double,0,1>& get_weights (
        ) const { return weights; }

        unsigned long num_labels (
        ) const { return fe.num_labels(); }

        labeled_sequence_type operator() (
            const sample_sequence_type& x
        ) const
        {
            // make sure requires clause is not broken
            DLIB_ASSERT(num_labels() > 0,
                "\t labeled_sequence_type sequence_labeler::operator()(x)"
                << "\n\t You can't have no labels."
                << "\n\t this: " << this
                );

            labeled_sequence_type y;
            find_max_factor_graph_viterbi(map_prob(x,fe,weights), y);
            return y;
        }

        void label_sequence (
            const sample_sequence_type& x,
            labeled_sequence_type& y
        ) const
        {
            // make sure requires clause is not broken
            DLIB_ASSERT(num_labels() > 0,
                "\t void sequence_labeler::label_sequence(x,y)"
                << "\n\t You can't have no labels."
                << "\n\t this: " << this
                );

            find_max_factor_graph_viterbi(map_prob(x,fe,weights), y);
        }

    private:

        feature_extractor fe;
        matrix<double,0,1> weights;
    };

// ----------------------------------------------------------------------------------------

    template <
        typename feature_extractor
        >
    void serialize (
        const sequence_labeler<feature_extractor>& item,
        std::ostream& out
    )
    {
        serialize(item.get_feature_extractor(), out);
        serialize(item.get_weights(), out);
    }

// ----------------------------------------------------------------------------------------

    template <
        typename feature_extractor
        >
    void deserialize (
        sequence_labeler<feature_extractor>& item,
        std::istream& in 
    )
    {
        feature_extractor fe;
        matrix<double,0,1> weights;

        deserialize(fe, in);
        deserialize(weights, in);

        item = sequence_labeler<feature_extractor>(fe, weights);
    }

// ----------------------------------------------------------------------------------------

}

#endif // DLIB_SEQUENCE_LAbELER_H___

