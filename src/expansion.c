/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jasminelager <jasminelager@student.42.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/05 12:50:57 by jlager            #+#    #+#             */
/*   Updated: 2025/08/13 13:03:23 by jasminelage      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// The expander handles variable substitution, tilde expansion,
// and quote removal before execution. This stage processes $HOME, $?,
// and environment variables.

// Expansion Tasks (Variable Processing):
// Environment variable substitution ($VAR)  - use find_env_var from initialize_var.c
//DO NOT expand anything in delimiter, do not do variable substitution for delimiter
// DO mark if each token was quoted
// Exit status expansion ($?)
// Home directory expansion (~)
// Quote removal after expansion


